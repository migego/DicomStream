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


ev_async my_eio_sig;

void my_eio_sig_cb (EV_P_ ev_async *w, int revents)
{
    if  (eio_poll())
        ev_async_send (EV_DEFAULT_ &my_eio_sig);
}



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

    //listen for eio events
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


int DicomStream::open_cb_(eio_req *req)
{
	int fd = EIO_RESULT (req);
	if (fd != -1)
	{
		TEio* data = (TEio*)req->data;
		data->fileInfo->fd = fd;

		//trigger readahead
		if (!data->fileInfo->parser)
			eio_readahead (fd, 0, 16448, 0, readahead_cb, req->data);
		else
			triggerNextEvent(data->cli);
	}

   return 0;
}

int DicomStream::readahead_cb_(eio_req *req)
{

	if (req->result < 0)
	  abort ();

	TEio* data = (TEio*)req->data;
	//parse file
	data->fileInfo->parser = new DicomPixels();
	data->fileInfo->parser->parse(data->fileInfo->fd, data->fileInfo->fileName, &listenManager);

	printf("[Server] readahead triggered write on file %s\n", data->fileInfo->fileName.c_str());
	TClient* cli = data->cli;
	delete(data);

	//trigger posix_fadvise(....SEQUENTIAL) on entire file
	eio_custom(eio_fadvise, 0, eio_fadvise_cb, new TFadvise(data->fileInfo->fd, 0, 0, POSIX_FADV_SEQUENTIAL));

	//trigger write on next fragment
	struct ev_loop *loop = ev_default_loop (0);
	triggerWrite(loop, cli);
	return 0;
}

int DicomStream::sendfile_cb_(eio_req *req)
{
	TEio* data = (TEio*)req->data;
	TClient* cli = data->cli;

	if (req->result == -1)
	{
		if (errno == EAGAIN)
		{
			printf("[server] sendfile return EAGAIN\n");
			eio_sendfile (cli->fd, data->fileInfo->fd, data->offset, data->size, 0, sendfile_cb, (void*)data);
			return 0;
		}
		perror("[server] sendfile error\n");
		abort ();
	}

	printf("[server] sent %d pixels\n", req->result);
	data->frameInfo->offset += req->result;

	if ( ((unsigned int)req->result) < data->size)
	{
		data->offset += req->result;
		data->size -= req->result;
		eio_sendfile (cli->fd, data->fileInfo->fd, data->offset, data->size, 0, sendfile_cb, (void*)data);
		return 0;
	}
	//clean up memory
	delete data;

	//trigger write on next fragment
	if (clientInfoMap.find(cli->fd) != clientInfoMap.end())
 	{
 		queue<FrameGroupIterator*>* frameQueue = &clientInfoMap[cli->fd]->frameGroupQueue;

 		//move front iterator to done state if it does not have a next fragment
 		frameQueue->front()->completeNext();

 		// remove done iterator
 		if (frameQueue->front()->isDone())
 		{
 			delete frameQueue->front();
 			frameQueue->pop();
 			printf("[server] popped queue; queue size is %d\n",frameQueue->size());
 		}


 		if (frameQueue->empty())
 		{
 			printf("[server] queue is empty\n");
 			//cleanup(cli);
 			setCork(cli->fd, false);
			return 0;
 		}

 		triggerNextEvent(cli);
 	}
	return 0;
}

void DicomStream::write_cb_(struct ev_loop *loop, struct ev_io *w, int revents)
{
    TClient *cli= ((TClient*) (((char*)w) - offsetof(struct TClient,ev_write)));
    // stop current write event before issuing new one
	ev_io_stop(EV_A_ w);

 	if (revents & EV_WRITE)
 	{
 		if (clientInfoMap.find(cli->fd) != clientInfoMap.end())
 		{
 			// queue must not be empty, because we do not trigger a write on an empty queue
 			queue<FrameGroupIterator*>* frameQueue = &clientInfoMap[cli->fd]->frameGroupQueue;

 			//frame info cannot be null, because we do not trigger a write on an
 			// uninitialized or completed FrameGroupIterator
			TFrameInfo* frameInfo =  frameQueue->front()->currentIterator()->getFrameInfo();

			//1. write frame header, if necessary
			TFileInfo* fileInfo = getFileInfo(frameInfo->fileName);
			if (!frameInfo->sentFrameHeader)
			{
				Protocol::FrameResponse frameResponse;
				frameResponse.set_instanceuid("hihi");
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
				    needsAnotherWrite = !messageFramers[cli->fd]->write(MessageFramer::FrameResponse, &frameResponse);
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

			//2. write fragment header and trigger fragment sendfile
			// next must return true, since we do not trigger write on completed FrameGroupIterator??

			Protocol::FrameFragmentHeader& currentFragment = clientInfoMap[cli->fd]->currentFragment;
			TFrameFragment frameFragment;
			bool needsWrite = messageFramers[cli->fd]->IsWriteInProgress();
			if (!needsWrite)
			{
				needsWrite = frameQueue->front()->next(frameFragment);
				if (needsWrite)
				{
                    currentFragment.set_offset(frameInfo->offset);
                    currentFragment.set_size(frameFragment.size);
				}
			}
			if (needsWrite)
			{
				bool needsAnotherWrite = false;
                try
                {
				   needsAnotherWrite = !messageFramers[cli->fd]->write(MessageFramer::FrameFragmentHeader, &currentFragment);
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

				TEio* data = new TEio();
				data->cli = cli;
				data->fileInfo = fileInfo;
			    data->frameInfo = frameInfo;
				data->offset = frameFragment.offset;
				data->size = frameFragment.size;
				//3. trigger eio sendfile on fragment
				printf("[server] sending pixels: offset %d, size %d\n", currentFragment.offset(), currentFragment.size());
				eio_sendfile (data->cli->fd, data->fileInfo->fd, data->offset, data->size, 0, sendfile_cb, (void*)data);
			}
			else
			{
                printf("[server] warning: front of frame queue has no next item\n");
			}
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
			if (messageFramers[cli->fd]->read(wrapper))
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

		//trigger another read
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
    TClient* cli = new TClient;
	cli->fd=client_fd;
	if (setNonBlock(cli->fd) < 0)
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
	close(cli->fd);

	//delete message framer
	deleteMessageFramer(cli->fd);

	//delete client queue
	if (clientInfoMap.find(cli->fd) != clientInfoMap.end())
	{
		TClientInfo* queueInfo = clientInfoMap[cli->fd];
		queue<FrameGroupIterator*>*  frameQueue = &queueInfo->frameGroupQueue;
		while(!frameQueue->empty())
		{
			delete frameQueue->front();
			frameQueue->pop();

		}
		delete queueInfo;
		clientInfoMap.erase(cli->fd);

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
				::close(info->fd);
		   info->fd = -1;
	   }
   }
}

void DicomStream::triggerNextEvent(TClient* cli)
{

	if (clientInfoMap.find(cli->fd) == clientInfoMap.end())
		return;
    queue<FrameGroupIterator*>* frameQueue = &clientInfoMap[cli->fd]->frameGroupQueue;
    if (frameQueue->empty())
    	return;
    FrameIterator* frameIter  = frameQueue->front()->currentIterator();

	string fileName = frameIter->getFrameInfo()->fileName;
	TFileInfo* info = getFileInfo(fileName);
	// trigger open file (which will initialize the FrameIterator)
	if (info->fd == -1)
	{
		TEio* eioData = new TEio();
		eioData->cli = cli;
		eioData->fileInfo = info;
		printf("[Server] trigger open on file %s\n", fileName.c_str());
		eio_open (fileName.c_str(), O_RDONLY, 0777, 0, open_cb, (void*)eioData);
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

void DicomStream::createMessageFramer(int fd)
{
	deleteMessageFramer(fd);
	messageFramers[fd] = new MessageFramer(fd);
}
void DicomStream::deleteMessageFramer(int fd)
{
	if ( messageFramers.find(fd) != messageFramers.end() )
	{
		delete messageFramers[fd];
		messageFramers.erase(fd);
	}
}

void  DicomStream::processIncomingMessage(DicomStream::TClient* cli, MessageFramer::MessageWrapper msg)
{
	if (msg.message == NULL)
		return;
	switch(msg.type)
	{
	case MessageFramer::SetPrimaryIndexRequest:
		{
		Protocol::SetPrimaryIndexRequest* indexRequest = static_cast<Protocol::SetPrimaryIndexRequest*>(msg.message);
		//find matching series, and possibly instance (for multiframe)

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
		queue<FrameGroupIterator*>* frameQueue = NULL;
		if (clientInfoMap.find(cli->fd) == clientInfoMap.end())
		{
			TClientInfo* queueInfo = new TClientInfo();
			clientInfoMap[cli->fd] = queueInfo;
			frameQueue = &queueInfo->frameGroupQueue;
		}
		else
		{
			frameQueue = &clientInfoMap[cli->fd]->frameGroupQueue;
		}

		FrameGroupIterator* frameIter = new FrameGroupIterator(itms, 0);
		bool emptyQueue = frameQueue->empty();
		frameQueue->push(frameIter);

		//trigger retrieve, if the queue is empty
		if (emptyQueue)
		{
			setCork(cli->fd, true);
			triggerNextEvent(cli);

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
	MessageFramer* framer = new MessageFramer(clientFd);

	Protocol::FrameResponse* frameResponse;
	Protocol::FrameFragmentHeader* frameFragment;

	int totalBytes = -1;
	int totalCount = 0;
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
			totalBytes = frameResponse->totalbytes();
			printf("[client] received frame header: total bytes: %d\n",totalBytes);
			break;
		case MessageFramer::FrameFragmentHeader:
			{
			frameFragment = (Protocol::FrameFragmentHeader*)wrapper.message;
			unsigned int size = frameFragment->size();
			unsigned int offset = frameFragment->offset();
			printf("[client] fragment header: offset %d, size %d\n",offset, size);
			unsigned int count = 0;
			char buffer[256];
			//read pixels
			while (count < size)
			{
				int maxBytes = MIN(256, size-count);
				int n = ::read(clientFd, buffer, maxBytes);
				if (n < 0)
				{
					perror("[client] ERROR reading pixels from socket\n");
					return;
				}
				count += n;
			}
			totalCount += count;
			printf("[client] read pixels: offset %d, size %d, totalCount %d, totalBytes %d\n",offset, count, totalCount, totalBytes);
			if (totalCount == totalBytes)
			{
				delete frameResponse;
				frameResponse=NULL;
				delete frameFragment;
				frameFragment = NULL;
				totalCount=0;
				totalBytes=0;
			}
			}

			break;
		case MessageFramer::None:
		case MessageFramer::FrameGroupRequest:
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
	req->set_priority(Protocol::FrameGroupRequest_Priority_Selected);
	req->set_instanceuidprefix("");

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

	clientFd = socket(AF_INET, SOCK_STREAM, 0);
	if (clientFd < 0)
		perror("ERROR opening socket");

	/* Disable the Nagle (TCP No Delay) algorithm */
	int flag = 1;
	int ret = setsockopt( clientFd, IPPROTO_TCP, TCP_NODELAY, (char *)&flag, sizeof(flag) );
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
	if (connect(clientFd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
		perror("ERROR connecting");

	//launch socket read thread
	    boost::thread clientTestThread(clientTestRead);

    // send requests
	MessageFramer* framer = new MessageFramer(clientFd);
	for (int i = 0; i < 2; ++i)
	{
		req->set_seriesuid(series[i].c_str());
		while (!framer->write(MessageFramer::FrameGroupRequest, req))
		{

		}
		printf("[client] sent frame group request\n");
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
	ev_io_init(&cli->ev_write,write_cb,cli->fd,EV_WRITE);
	ev_io_start(loop,&cli->ev_write);
}
void DicomStream::triggerRead(struct ev_loop *loop, TClient* cli)
{
	ev_io_init(&cli->ev_read,read_cb,cli->fd,EV_READ);
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






