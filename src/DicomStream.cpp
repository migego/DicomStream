/*
 * DicomStream.cpp
 *
 *  Created on: Jun 6, 2011
 *      Author: aboxer
 */

using namespace std;

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>
#include <errno.h>
#include <err.h>
#include <stddef.h>

#include <boost/thread.hpp>
#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/info_parser.hpp>
#include <string>
#include <vector>
#include "DicomStream.h"

#define CLIENT_TEST


DicomStream* DicomStream::instance=NULL;

DicomStream::DicomStream() : path("")
{
}

DicomStream::~DicomStream() {

}


DicomStream* DicomStream::Instance()
{
	if (instance == NULL)
		instance = new DicomStream();
	return instance;

}


void DicomStream::start()
{

	// read config file
	boost::property_tree::ptree pTree;
	try {
		boost::property_tree::read_info("src/DicomStream.cfg", pTree);
	}
	catch (boost::property_tree::info_parser_error &e) {
		std::cout << "error" << std::endl;
	}
	try {
		port = pTree.get<int>("server.port");
		path = pTree.get<string>("storage.path");
	}
	catch(boost::property_tree::ptree_bad_path &e) {
		std::cout << "error" << std::endl;
	}

    // initialize EIO
    if (eio_init (want_poll, done_poll)) abort ();

	//create listen socket
    sockaddr_in listen_addr;
    int reuseaddr_on = 1;
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd < 0)
            err(1, "listen failed");
    if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &reuseaddr_on, sizeof(reuseaddr_on)) == -1)
            err(1, "setsockopt failed");
    memset(&listen_addr, 0, sizeof(listen_addr));
    listen_addr.sin_family = AF_INET;
    listen_addr.sin_addr.s_addr = INADDR_ANY;
    listen_addr.sin_port = htons(port);
    if (bind(listen_fd, (struct sockaddr *)&listen_addr, sizeof(listen_addr)) < 0)
            err(1, "bind failed");
    if (listen(listen_fd,5) < 0)
            err(1, "listen failed");
    if (setNonBlock(listen_fd) < 0)
            err(1, "failed to set server socket to non-blocking");

    //listen to socket
	ev_io_init(&ev_accept,accept_cb,listen_fd,EV_READ);
    struct ev_loop *loop = ev_default_loop (0);
	ev_io_start(loop,&ev_accept);

    //listen for eio events in libev event loop
    ev_async_init (&my_eio_sig, my_eio_sig_cb);
    ev_async_start(loop, &my_eio_sig);

    //start client test thread
#ifdef CLIENT_TEST
    boost::thread clientTestThread(clientTest);
#endif


	ev_loop (loop, 0);
}

void DicomStream::want_poll_ ()
{
     //fire asynch event
    ev_async_send (EV_DEFAULT_ &my_eio_sig);
}

void DicomStream::done_poll_ ()
{

}

void DicomStream::my_eio_sig_cb_ (EV_P_ ev_async *w, int revents)
{
    if  (eio_poll())
        ev_async_send (EV_DEFAULT_ &my_eio_sig);
}


int DicomStream::setNonBlock(int fd)
{
    int flags = fcntl(fd, F_GETFL);
    if (flags < 0)
            return flags;
    flags |= O_NONBLOCK;
    if (fcntl(fd, F_SETFL, flags) < 0)
            return -1;

    return 0;
}

//TCP_CORK for efficient sending of small + large amounts of data
int DicomStream::setCork(int clientFd, bool cork)
{
    int rc = 0;
    int state = cork ? 1 : 0;
    rc = setsockopt(clientFd, IPPROTO_TCP, TCP_CORK, &state, sizeof(state));
    if (rc  == -1)
    	perror("[server] failed to set cork on socket for cork state");
    return rc;
}

void DicomStream::eio_fadvise_(eio_req *req)
{
	TFadvise* info  = (TFadvise*)req->data;
    posix_fadvise(info->fd, info->offset, info->len, info->advice);
	delete info;
}
int DicomStream::eio_fadvise_cb_(eio_req *req)
{
	return 0;
}

int DicomStream::close_cb_(eio_req *req)
{
	int res = EIO_RESULT (req);
	if (res != -1)
	{
		TFileInfo* info =  (TFileInfo*)req->data;
		info->fd = -1;
		//if (info->nextPending == FILE_OPEN)
		//	open
		info->pending = FILE_NONE;
	}

   return 0;
}

int DicomStream::open_cb_(eio_req *req)
{
	int fd = EIO_RESULT (req);
	if (fd != -1)
	{
		TEioData* data = (TEioData*)req->data;
		TFileInfo* fileInfo = getFileInfo(data->frameGroup);
		fileInfo->fd = fd;
		fileInfo->pending = FILE_NONE;
		release(fileInfo->fileName);

		//trigger readahead
		if (!fileInfo->parser)
			eio_readahead (fd, 0, 16448, 0, readahead_cb, req->data);
		else
			triggerOpenOrWrite(data->socketFd);
	}

   return 0;
}

int DicomStream::readahead_cb_(eio_req *req)
{

	if (req->result < 0)
	  abort ();

	TEioData* data = (TEioData*)req->data;
	//parse file
	TFileInfo* fileInfo = getFileInfo(data->frameGroup);
	fileInfo->parser = new DicomPixels();
	fileInfo->parser->parse(fileInfo->fd, fileInfo->fileName, &listenManager);

	printf("[Server] readahead triggered write on file %s\n", fileInfo->fileName.c_str());
	//trigger posix_fadvise(....SEQUENTIAL) on entire file
	eio_custom(eio_fadvise, 0, eio_fadvise_cb, new TFadvise(fileInfo->fd, 0, 0, POSIX_FADV_SEQUENTIAL));

	//trigger write on next fragment
	struct ev_loop *loop = ev_default_loop (0);
	triggerWrite(loop,  new TClient(data));
	delete data;
	return 0;
}

int DicomStream::sendfile_cb_(eio_req *req)
{
	TEioData* data = (TEioData*)req->data;
    TFileInfo* fileInfo = getFileInfo(data->frameGroup);
	if (req->result == -1)
	{
		if (errno == EAGAIN)
		{
			printf("[server] sendfile return EAGAIN\n");
			eio_sendfile (data->socketFd, fileInfo->fd, data->offset, data->size, 0, sendfile_cb, (void*)data);
			return 0;
		}
		perror("[server] sendfile error\n");
		abort ();
	}

	printf("[server] sent %d pixels\n", req->result);
	data->frameGroup->currentIterator()->getFrameInfo()->offset += req->result;

	if ( ((unsigned int)req->result) < data->size)
	{
		data->offset += req->result;
		data->size -= req->result;
		eio_sendfile (data->socketFd, fileInfo->fd, data->offset, data->size, 0, sendfile_cb, (void*)data);
		return 0;
	}

	//move front iterator to done state if it does not have a next fragment
	data->frameGroup->completeNext();

	// trigger next event
	triggerOpenOrWrite(data->socketFd);

	//clean up memory
	delete data;


	return 0;
}

void DicomStream::write_cb_(struct ev_loop *loop, struct ev_io *w, int revents)
{
    TClient *cli= ((TClient*) (((char*)w) - offsetof(struct TClient,ev_write)));
    // stop current write event before issuing new one
	ev_io_stop(EV_A_ w);

 	if (revents & EV_WRITE)
 	{

		TFrameInfo* frameInfo =  cli->frameGroup->currentIterator()->getFrameInfo();

		//1. write frame header, if necessary
		TFileInfo* fileInfo = getFileInfo(frameInfo->fileName);
		if (!frameInfo->sentFrameHeader)
		{
			Protocol::FrameResponse frameResponse;
			frameResponse.set_instanceuid(frameInfo->frameRequest.instanceuid());
			frameResponse.set_framenumber(0);

			DicomPixels* parser = fileInfo->parser;
			if (parser)
			{
				frameResponse.set_totalbytes(frameInfo->size);
				frameResponse.set_imagesizex(parser->getimageSizeX());
				frameResponse.set_imagesizey(parser->getimageSizeY());
				frameResponse.set_depth((Protocol::FrameResponse_bitDepth)parser->getdepth());
				frameResponse.set_colorspace(parser->getcolorSpace());
				frameResponse.set_transfersyntax(parser->gettransferSyntax());
				frameResponse.set_channelsnumber(parser->getchannelsNumber());
				frameResponse.set_binterleaved(parser->getbInterleaved());
				frameResponse.set_b2complement(parser->getb2Complement());
				frameResponse.set_allocatedbits(parser->getallocatedBits());
				frameResponse.set_storedbits(parser->getstoredBits());
				frameResponse.set_highbit(parser->gethighBit());

			}
			bool needsAnotherWrite = false;
			try
			{
				printf("[server] sending frame header: totalBytes : %d\n",frameInfo->size);
				needsAnotherWrite = !getMessageFramer(cli->socketFd)->write(MessageFramer::FrameResponse, &frameResponse);
			}
			catch (EAgainException& ee)
			{
				needsAnotherWrite = true;

			}
			catch (WriteException& we)
			{
				perror("[server] write exception\n");
				cleanup(cli);
				return;
			}
			//trigger another write
			if (needsAnotherWrite)
			{
				triggerWrite(loop, cli);
				return;
			}

			frameInfo->sentFrameHeader = true;

		}

		//2. if we are not in the middle of a write, and there is a pending priority change,
		// then we need to triger a new open or write
		processPendingSetPriorityRequest(cli->socketFd);
		bool needsWrite = getMessageFramer(cli->socketFd)->IsWriteInProgress();
		if (!needsWrite && cli->frameGroup != clientInfoMap[cli->socketFd]->frameGroupQueue.front())
		{
			//priority has changed: trigger
			triggerOpenOrWrite(cli->socketFd);
			delete cli;
			return;
		}


		//3. write fragment header and trigger fragment sendfile
		// next must return true, since we do not trigger write on completed FrameGroupIterator??

		Protocol::FrameFragmentHeader& fragmentHeader = clientInfoMap[cli->socketFd]->currentFragment;
		TFrameFragment frameFragment;
		if (!needsWrite)
		{
			needsWrite = cli->frameGroup->next(frameFragment);
			if (needsWrite)
			{
				fragmentHeader.set_instanceuid(frameInfo->frameRequest.instanceuid());
				fragmentHeader.set_offset(frameInfo->offset);
				fragmentHeader.set_size(frameFragment.size);
			}
		}
		if (needsWrite)
		{
			bool needsAnotherWrite = false;
			try
			{
			   needsAnotherWrite = !getMessageFramer(cli->socketFd)->write(MessageFramer::FrameFragmentHeader, &fragmentHeader);
			}
			catch (EAgainException& ee)
			{
				needsAnotherWrite = true;

			}
			catch (WriteException& we)
			{
				perror("[server] write exception\n");
				cleanup(cli);
				return;
			}

			if (needsAnotherWrite)
			{
				//trigger another write
				triggerWrite(loop,cli);
				return;

			}

			TEioData* data = new TEioData(cli, frameFragment.offset, frameFragment.size);
			//3. trigger eio sendfile on fragment
			printf("[server] sending pixels: offset %d, size %d\n", fragmentHeader.offset(), fragmentHeader.size());
			eio_sendfile (data->socketFd, fileInfo->fd, data->offset, data->size, 0, sendfile_cb, (void*)data);
		}
		else
		{
			printf("[server] warning: front of frame queue has no next item\n");
		}
	}
}


 void DicomStream::read_cb_(struct ev_loop *loop, struct ev_io *w, int revents)
{
	ev_io_stop(EV_A_ w);
    TClient *cli= ((TClient*) (((char*)w) - offsetof(TClient,ev_read)));
	if (revents & EV_READ){
		printf("[server] read\n");
		// parse message
		MessageFramer::MessageWrapper wrapper;
		try
		{
			if (getMessageFramer(cli->socketFd)->read(wrapper))
			{
				//process message
				processIncomingMessage(cli, wrapper);
			}
		}
		catch (ReadException& re)
		{
			cleanup(cli);
			return;
		}
		catch (EAgainException& ee)
		{
		}

		//trigger another read (re-use same TClient struct)
		triggerRead(loop,cli);

	}


}

void DicomStream::accept_cb_(struct ev_loop *loop, struct ev_io *w, int revents)
{
    sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
	int client_fd = accept(w->fd, (sockaddr *)&client_addr, &client_len);
	if (client_fd == -1) {
			return;
	}
    TClient* cli = new TClient(client_fd);
	if (setNonBlock(cli->socketFd) < 0)
	{
         err(1, "failed to set client socket to non-blocking");
         close(client_fd);
         return;
	}
	createMessageFramer(client_fd);
	triggerRead(loop,cli);
}

void DicomStream::cleanup(TClient* cli)
{
	printf("[server] cleanup client\n");
	//close socket
	close(cli->socketFd);

	//delete message framer
	deleteMessageFramer(cli->socketFd);

	//delete client queue
	if (clientInfoMap.find(cli->socketFd) != clientInfoMap.end())
	{
		TClientInfo* queueInfo = clientInfoMap[cli->socketFd];
		vector<FrameGroupIterator*>*  frameQueue = &queueInfo->frameGroupQueue;
		while(!frameQueue->empty())
		{
			delete frameQueue->front();
			frameQueue->erase(frameQueue->begin() );

		}
		delete queueInfo;
		clientInfoMap.erase(cli->socketFd);

	}

	delete cli;
}

void DicomStream::cleanup(string fileName)
{
   if (fileInfo.find(fileName) != fileInfo.end())
   {
	   TFileInfo* info = fileInfo[fileName];
	   if (info)
	   {
		   if (info->fd != -1)
		   {
			  if (info->pending == FILE_NONE)
			  {
				 info->pending = FILE_CLOSE;
			     eio_close (info->fd, 0, close_cb, (void*)info);
			  }
			  else if (info->pending == FILE_OPEN)
			  {
				  info->nextPending = FILE_CLOSE;
			  }
		   }
	   }
   }
}

void DicomStream::triggerOpenOrWrite(int clientFd)
{
	if (clientInfoMap.find(clientFd) == clientInfoMap.end())
		return;
    vector<FrameGroupIterator*>* frameQueue = &clientInfoMap[clientFd]->frameGroupQueue;
    if (frameQueue->empty())
    	return;

    while (!frameQueue->empty() && frameQueue->front()->isDone())
    {
    	frameQueue->erase(frameQueue->begin());
		printf("[server] popped queue; queue size is %d\n",frameQueue->size());
    }

	if (frameQueue->empty())
	{
		printf("[server] queue is empty\n");
		//cleanup(cli);
		setCork(clientFd, false);
		return;
	}

    TClient* cli = new TClient(clientFd, frameQueue->front());
    FrameIterator* frameIter  = cli->frameGroup->currentIterator();
	string fileName = frameIter->getFrameInfo()->fileName;
	TFileInfo* info = getFileInfo(fileName);

	// trigger open file (which will initialize the FrameIterator)
	if (info->fd == -1)
	{
		open(cli, fileName);
	}
	//trigger write of next fragment
	else
	{
		//initialize FrameIterator, if file is already open
		if (!frameIter->isInitialized())
		{
			info->parser->notify(frameIter);
		}

		printf("[Server] trigger write on file %s\n", fileName.c_str());
		struct ev_loop *loop = ev_default_loop (0);
		triggerWrite(loop,cli);

	}
}

void DicomStream::open(TClient* cli, string fileName)
{
	TFileInfo* info = getFileInfo(fileName);

	// trigger open file (which will initialize the FrameIterator)
	if (info->fd == -1)
	{
		if (info->pending == FILE_NONE)
		{
			acquire(fileName);
			TEioData* eioData = new TEioData(cli);
			printf("[Server] trigger open on file %s\n", fileName.c_str());
			info->pending = FILE_OPEN;
			eio_open (fileName.c_str(), O_RDONLY, 0777, 0, open_cb, (void*)eioData);
		}
		else if (info->pending == FILE_CLOSE)
		{
			info->nextPending = FILE_OPEN;
		}
	}
}

MessageFramer* DicomStream::getMessageFramer(int fd)
{
	if ( clientInfoMap.find(fd) != clientInfoMap.end() )
	{
		return clientInfoMap[fd]->messageFramer;
	}
	return NULL;

}
void DicomStream::createMessageFramer(int fd)
{
	deleteMessageFramer(fd);
	TClientInfo* info = NULL;
	if (clientInfoMap.find(fd) == clientInfoMap.end())
	{
		info = new TClientInfo();
		clientInfoMap[fd] = info;
	}
	else
	{
		info = clientInfoMap[fd];

	}
	info->messageFramer = new MessageFramer(fd);
}
void DicomStream::deleteMessageFramer(int fd)
{
	if ( clientInfoMap.find(fd) != clientInfoMap.end() )
	{
		TClientInfo* info = clientInfoMap[fd];
		if (info->messageFramer != NULL)
		{
		    delete info->messageFramer;
		    info->messageFramer = NULL;
		}
	}
}

void DicomStream::processPendingSetPriorityRequest(int clientFd)
{
	//find matching series, and possibly instance (for multiframe)
	if (clientInfoMap.find(clientFd) != clientInfoMap.end())
	{
		TClientInfo* clientInfo = clientInfoMap[clientFd];
		string newSeries = clientInfo->pendingSeriesInstanceUid;
		if (newSeries == "")
			return;

		//assume that front of queue is the group that needs its index changed !!!
		if (!clientInfo->frameGroupQueue.empty())
		{
			vector<FrameGroupIterator*>* frameQueue =  &clientInfo->frameGroupQueue;
			vector<FrameGroupIterator*>::iterator iter = frameQueue->begin();
			while (iter != frameQueue->end())
			{
				if ((*iter)->getSeriesInstanceUid() == newSeries)
				{
					if (iter != frameQueue->begin())
					{
						frameQueue->erase(iter);
						frameQueue->insert(frameQueue->begin(), *iter);
					}
					printf("[server] processed priority request\n");
					break;
				}
				iter++;
			}
		}
		clientInfo->pendingSeriesInstanceUid ="";

	}
}

void  DicomStream::processIncomingMessage(DicomStream::TClient* cli, MessageFramer::MessageWrapper msg)
{
	if (msg.message == NULL)
		return;
	switch(msg.type)
	{
	case MessageFramer::SetPriorityRequest:
		{
		Protocol::SetPriorityRequest* priorityRequest = static_cast<Protocol::SetPriorityRequest*>(msg.message);
		if (clientInfoMap.find(cli->socketFd) != clientInfoMap.end())
		{
			TClientInfo* clientInfo = clientInfoMap[cli->socketFd];
			clientInfo->pendingSeriesInstanceUid = priorityRequest->seriesuid();
		}

		}

		break;
	case MessageFramer::SetPrimaryIndexRequest:
		{
		Protocol::SetPrimaryIndexRequest* indexRequest = static_cast<Protocol::SetPrimaryIndexRequest*>(msg.message);
		printf("[server] received primary index request\n");

		//find matching series, and possibly instance (for multiframe)
		if (clientInfoMap.find(cli->socketFd) != clientInfoMap.end())
		{
			TClientInfo* clientInfo = clientInfoMap[cli->socketFd];

			//assume that front of queue is the group that needs its index changed !!!
			if (!clientInfo->frameGroupQueue.empty())
			{
				clientInfo->frameGroupQueue.front()->setPrimaryIndex(indexRequest->primaryindex());
			}
		}

		}
		break;
	case MessageFramer::FrameGroupRequest:
	    {
	    Protocol::FrameGroupRequest* frameGroupRequest = static_cast<Protocol::FrameGroupRequest*>(msg.message);
		string fileRoot = path
								  + "/" + frameGroupRequest->studyuid()
								  + "/" + frameGroupRequest->seriesuid()
								  + "/" + frameGroupRequest->instanceuidprefix();

	    ::google::protobuf::RepeatedPtrField< ::Protocol::FrameRequest >::const_iterator framesIter = frameGroupRequest->frames().begin();
        if (frameGroupRequest->frames().size() == 0)
        {
        	printf("[server] tried to process incoming message with zero frames\n");
        	return;
        }

        printf("[server] processed incoming message \n");
	    vector<FrameIterator*>* itms = new vector<FrameIterator*>();
	    while (framesIter != frameGroupRequest->frames().end())
		{
			string fileName = fileRoot + framesIter->instanceuid() + ".dcm";
			TFrameInfo info;
			info.fileName = fileName;
			info.frameRequest = *framesIter;
			itms->push_back(new FrameIterator(this, &listenManager, info));
			framesIter++;
		}

		//queue new iterator
		vector<FrameGroupIterator*>* frameQueue = NULL;
		if (clientInfoMap.find(cli->socketFd) == clientInfoMap.end())
		{
			TClientInfo* queueInfo = new TClientInfo();
			clientInfoMap[cli->socketFd] = queueInfo;
			frameQueue = &queueInfo->frameGroupQueue;
		}
		else
		{
			frameQueue = &clientInfoMap[cli->socketFd]->frameGroupQueue;
		}

		FrameGroupIterator* frameIter = new FrameGroupIterator(itms, 0, frameGroupRequest->studyuid(), frameGroupRequest->seriesuid());
		bool wasEmptyQueue = frameQueue->empty();
		frameQueue->push_back(frameIter);

		//trigger retrieve, if the queue is empty
		if (wasEmptyQueue)
		{
			setCork(cli->socketFd, true);
			triggerOpenOrWrite(cli->socketFd);
		}

	    }
		break;
	default:
		break;
	}
	delete msg.message;

}


void DicomStream::clientTestRead_()
{
	MessageFramer* framer = new MessageFramer(testClientSocketFd);

	Protocol::FrameResponse* frameResponse;
	Protocol::FrameFragmentHeader* frameFragment;
	while(true)
	{

		//read message
		MessageFramer::MessageWrapper wrapper;
		try
		{
			if (!framer->read(wrapper))
				continue;
		}
		catch (ReadException &r)
		{
			perror("[client] ERROR reading header from socket\n");
			return;
		}
		switch(wrapper.type)
		{
		case MessageFramer::FrameResponse:
			frameResponse = (Protocol::FrameResponse*)wrapper.message;
			printf("[client] received frame header: total bytes: %d\n",frameResponse->totalbytes());
			break;
		case MessageFramer::FrameFragmentHeader:
			{
			TClientResponseInfo* responseInfo = NULL;
			frameFragment = (Protocol::FrameFragmentHeader*)wrapper.message;
			string instanceUid = frameFragment->instanceuid();
			if (clientResponseInfoMap.find(instanceUid) != clientResponseInfoMap.end())
			{
				responseInfo = clientResponseInfoMap[instanceUid];
			}
			else
			{
				responseInfo = new TClientResponseInfo();
				responseInfo->frameFragment = frameFragment;
				responseInfo->frameResponse = frameResponse;
				clientResponseInfoMap[instanceUid] = responseInfo;

			}

			unsigned int size = frameFragment->size();
			unsigned int offset = frameFragment->offset();
			printf("[client] fragment header: offset %d, size %d\n",offset, size);
			unsigned int count = 0;
			char buffer[256];
			//read pixels
			while (count < size)
			{
				int maxBytes = MIN(256, size-count);
				int n = ::read(testClientSocketFd, buffer, maxBytes);
				if (n < 0)
				{
					perror("[client] ERROR reading pixels from socket\n");
					return;
				}
				count += n;
			}
			responseInfo->totalCount += count;
			printf("[client] read pixels: offset %d, size %d, totalCount %d, totalBytes %d\n",offset, count, responseInfo->totalCount, frameResponse->totalbytes());
			}

			break;
		default:
			break;
		}

	}

}

void DicomStream::clientTest_()
{
	string series[2] = {"series1", "series2"};

	Protocol::FrameGroupRequest* req = new Protocol::FrameGroupRequest();
	req->set_studyuid("study1");
	req->set_seriesuid("series1");


	Protocol::FrameRequest* frameReq = new Protocol::FrameRequest();
	frameReq->set_instanceuid("instance1");
	frameReq->set_framenumber(1);
	req->mutable_frames()->AddAllocated(frameReq);

	frameReq = new Protocol::FrameRequest();
	frameReq->set_instanceuid("instance2");
	frameReq->set_framenumber(1);
	req->mutable_frames()->AddAllocated(frameReq);

	frameReq = new Protocol::FrameRequest();
	frameReq->set_instanceuid("instance3");
	frameReq->set_framenumber(1);
	req->mutable_frames()->AddAllocated(frameReq);


	struct sockaddr_in serv_addr;

	testClientSocketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (testClientSocketFd < 0)
		perror("ERROR opening socket");

	/* Disable the Nagle (TCP No Delay) algorithm */
	int flag = 1;
	int ret = setsockopt( testClientSocketFd, IPPROTO_TCP, TCP_NODELAY, (char *)&flag, sizeof(flag) );
	if (ret == -1) {
	  printf("Couldn't setsockopt(TCP_NODELAY)\n");
	  exit( EXIT_FAILURE );
	}

	string host = "localhost";
	struct hostent* server = gethostbyname(host.c_str());
	if (server == NULL) {
		fprintf(stderr,"ERROR, no such host\n");
		exit(0);
	}
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr,
		 (char *)&serv_addr.sin_addr.s_addr,
		 server->h_length);
	serv_addr.sin_port = htons(port);
	if (connect(testClientSocketFd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
		perror("ERROR connecting");

	//launch socket read thread
	boost::thread clientTestThread(clientTestRead);

    // send requests
	MessageFramer* framer = new MessageFramer(testClientSocketFd);
	for (int n = 0; n < 20; ++n)
	{
		for (int i = 0; i < 2; ++i)
		{
			req->set_seriesuid(series[i].c_str());
			string prefix = "study1_" + series[i] + "_";
			req->set_instanceuidprefix(prefix.c_str());
			while (!framer->write(MessageFramer::FrameGroupRequest, req))
			{

			}
			printf("[client] sent frame group request\n");

			Protocol::SetPrimaryIndexRequest* indexReq = new Protocol::SetPrimaryIndexRequest();
			indexReq->set_studyuid("study1");
			indexReq->set_seriesuid(series[i].c_str());
			indexReq->set_primaryindex(1);
			while (!framer->write(MessageFramer::SetPrimaryIndexRequest, indexReq))
			{

			}

			Protocol::SetPriorityRequest* priorityReq = new Protocol::SetPriorityRequest();
			priorityReq->set_studyuid("study1");
			priorityReq->set_seriesuid(series[i].c_str());
			while (!framer->write(MessageFramer::SetPriorityRequest, priorityReq))
			{

			}
			printf("[client] sent set priority request\n");

		}
	}
	delete framer;

}


int DicomStream::acquire(string fileName)
{
	TFileInfo* info = getFileInfo(fileName);
	info->refCount++;
	printf("[server] acquired file %s; ref count = %d\n", fileName.c_str(), info->refCount);
	return info->refCount;
}
int DicomStream::release(string fileName)
{
	if (fileInfo.find(fileName) == fileInfo.end())
	{
		return -1;
	}
	TFileInfo* info = fileInfo[fileName];
	info->refCount--;
	printf("[server] released file %s; ref count = %d\n", fileName.c_str(), info->refCount);
	if (info->refCount == 0)
	{
		cleanup(info->fileName);
		return 0;
	}
	return info->refCount;
}

int DicomStream::refCount(string fileName)
{
	if (fileInfo.find(fileName) == fileInfo.end())
	{
		return -1;
	}
	return fileInfo[fileName]->refCount;
}

DicomStream::TFileInfo* DicomStream::getFileInfo(FrameGroupIterator* frameGroup)
{
   if (!frameGroup)
	   return NULL;
   string fileName = frameGroup->currentIterator()->getFrameInfo()->fileName;
   return getFileInfo(fileName);
}

 DicomStream::TFileInfo* DicomStream::getFileInfo(string fileName)
{
	TFileInfo* info = NULL;
	if (fileInfo.find(fileName) == fileInfo.end())
	{
		info = new TFileInfo();
		fileInfo[fileName] = info;
		info->fileName = fileName;
	}
	else
	{
		info = fileInfo[fileName];
	}
	return info;
}

void DicomStream::triggerWrite(struct ev_loop *loop, TClient* cli)
{
	ev_io_init(&cli->ev_write,write_cb,cli->socketFd,EV_WRITE);
	ev_io_start(loop,&cli->ev_write);
}
void DicomStream::triggerRead(struct ev_loop *loop, TClient* cli)
{
	ev_io_init(&cli->ev_read,read_cb,cli->socketFd,EV_READ);
	ev_io_start(loop,&cli->ev_read);
}




//// STATIC WRAPPER METHODS/////////////////



void DicomStream::want_poll()
{
	Instance()->want_poll_();
}
void DicomStream::done_poll()
{
	Instance()->done_poll_();
}

int DicomStream::open_cb (eio_req *req)
{
	return Instance()->open_cb_(req);
}
int DicomStream::close_cb (eio_req *req)
{
	return Instance()->close_cb_(req);
}
int DicomStream::readahead_cb (eio_req *req)
{
	return Instance()->readahead_cb_(req);
}
int DicomStream::sendfile_cb (eio_req *req)
{
	return Instance()->sendfile_cb_(req);
}
void DicomStream::write_cb(struct ev_loop *loop, struct ev_io *w, int revents)
{
	Instance()->write_cb_(loop,w,revents);
}

void DicomStream::read_cb(struct ev_loop *loop, struct ev_io *w, int revents)
{
   Instance()->read_cb_(loop,w,revents);
}
void DicomStream::accept_cb(struct ev_loop *loop, struct ev_io *w, int revents)
{
	Instance()->accept_cb_(loop, w, revents);
}
void DicomStream::my_eio_sig_cb (EV_P_ ev_async *w, int revents)
{
    Instance()->my_eio_sig_cb_(loop,w,revents);
}
void DicomStream::clientTest()
{

   Instance()->clientTest_();
}
void DicomStream::clientTestRead()
{

   Instance()->clientTestRead_();
}
 void DicomStream::eio_fadvise(eio_req *req)
{
	Instance()->eio_fadvise_(req);
}
 int DicomStream::eio_fadvise_cb(eio_req *req)
{
	return Instance()->eio_fadvise_cb_(req);
}

/////////////////////////////////////////////






