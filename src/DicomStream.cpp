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

DicomStream::DicomStream() : stopPrecache(false)
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


void DicomStream::unitTest()
{
    //start prefetch thread
    boost::thread workerThread(preFetch);

    vector< SimpleIterator<string>*  >* fragIterators = new vector< SimpleIterator<string>*  >();

	fragIterators->push_back(new SimpleIterator<string>("helloa1"));
	fragIterators->push_back(new SimpleIterator<string>("helloa2"));
	fragIterators->push_back(new SimpleIterator<string>("helloa3"));


    UpDownIterator< string, SimpleIterator<string> >* upDown = new UpDownIterator< string, SimpleIterator<string> >();
    upDown->setChildIterators(fragIterators, 1);
    // start pre-fetch on these files
    if ( !fragIterators->empty())
        precacheQueue.push(upDown);

    sleep(5000);

}

void DicomStream::start()
{
	//unitTest();

	// read config file
	boost::property_tree::ptree pTree;
	try {
		read_info("src/DicomStream.cfg", pTree);
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

    //start prefetch thread
    boost::thread workerThread(preFetch);

    // initialize EIO
    if (eio_init (want_poll, done_poll)) abort ();



    //start client test thread
#ifdef CLIENT_TEST
    boost::thread clientTestThread(clientTest);
#endif

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

//TCP_CORK for low=latency sending of large amounts of data
int DicomStream::setCork(int fd, bool cork)
{
    int rc = 0;
#ifdef LINUX
    int state = 0;
    rc = setsockopt(imageFileDescriptor, IPPROTO_TCP, TCP_CORK, &state, sizeof(state));

    state ~= state;
    setsockopt(imageFileDescriptor, IPPROTO_TCP, TCP_CORK, &state, sizeof(state));
#endif
    return rc;
}

int DicomStream::open_cb_(eio_req *req)
{
	int fd = EIO_RESULT (req);
	if (fd != -1)
	{
		TFileInfo* data = (TFileInfo*)req->data;
		data->fd = fd;

		//trigger readahead
		eio_readahead (fd, 0, 4096, 0, readahead_cb, req->data);
	}

   return 0;
}

int DicomStream::readahead_cb_(eio_req *req)
{

	if (req->result < 0)
	  abort ();

	TFileInfo* data = (TFileInfo*)req->data;
	//parse file
	data->parser = new DicomPixels();
	data->parser->parse(data->fd);

	return 0;

}



void DicomStream::write_cb_(struct ev_loop *loop, struct ev_io *w, int revents)
{
    TClient *cli= ((TClient*) (((char*)w) - offsetof(struct TClient,ev_write)));
 	if (revents & EV_WRITE)
 	{

 		//write message header

 		//trigger eio sendfile


	}
	ev_io_stop(EV_A_ w);
	deleteMessageFramer(cli->fd);
 	close(cli->fd);
	delete cli;

}


 void DicomStream::read_cb_(struct ev_loop *loop, struct ev_io *w, int revents)
{

    TClient *cli= ((TClient*) (((char*)w) - offsetof(TClient,ev_read)));
    MessageFramer::GenericMessage msg;
	if (revents & EV_READ){
		// parse message
		printf("Reading message \n");
		msg  = messageFramers[cli->fd]->read();
		if (msg.message != NULL)
		{
			//process message
			processIncomingMessage(cli->fd, msg);
		}

	}
	ev_io_stop(EV_A_ w);
	if (msg.message != NULL)
	{
		ev_io_init(&cli->ev_write,write_cb,cli->fd,EV_WRITE);
		ev_io_start(loop,&cli->ev_write);
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
    TClient* myClient = new TClient;
	myClient->fd=client_fd;
	if (setNonBlock(myClient->fd) < 0)
	{
         err(1, "failed to set client socket to non-blocking");
         close(client_fd);
         return;
	}


	createMessageFramer(client_fd);
	ev_io_init(&myClient->ev_read,read_cb,myClient->fd,EV_READ);
	ev_io_start(loop,&myClient->ev_read);
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

void  DicomStream::processIncomingMessage(int clientFd, MessageFramer::GenericMessage msg)
{
	if (msg.message == NULL)
		return;
	switch(msg.type)
	{
	case MessageFramer::FrameGroupRequest:
	    {
	    Protocol::FrameGroupRequest* frameGroupRequest = static_cast<Protocol::FrameGroupRequest*>(msg.message);
		string fileRoot = path
								  + "/" + frameGroupRequest->studyuid()
								  + "/" + frameGroupRequest->seriesuid()
								  + "/" + frameGroupRequest->instanceuidprefix();
	    vector< SimpleIterator<string>*  >* prefetchIterators = new vector< SimpleIterator<string>*  >();
	    ::google::protobuf::RepeatedPtrField< ::Protocol::FrameRequest >::const_iterator frames = frameGroupRequest->frames().begin();

	    //prefetch
	    while (frames != frameGroupRequest->frames().end())
		{
			string fileName = fileRoot + frames->instanceuid() + ".dcm";
			printf("Incoming request for file: %s\n",fileName.c_str());

			prefetchIterators->push_back(new SimpleIterator<string>(fileName));

			frames++;
		}
	    if ( !prefetchIterators->empty())
	    {
	    	UpDownIterator< string, SimpleIterator<string> >* iter = new UpDownIterator< string, SimpleIterator<string> >();
	    	iter->setChildIterators(prefetchIterators, prefetchIterators->size()/2);
	    	precacheQueue.push(iter);
	    }

	    if (frameGroupRequest->multiframe())
	    {

	    }
	    else
	    {
	    	//trigger file open
		    frames = frameGroupRequest->frames().begin();
		    while (frames != frameGroupRequest->frames().end())
		    {
			    string fileName = fileRoot + frames->instanceuid() + ".dcm";
			    printf("Incoming request for file: %s\n",fileName.c_str());

			    if (fileInfo.find(fileName) == fileInfo.end())
			    {
					TFileInfo* data = new TFileInfo();
					fileInfo[fileName] = data;
			    	eio_open (fileName.c_str(), O_RDONLY, 0777, 0, open_cb, (void*)data);
			    }
			    frames++;
		    }
	    }

	    }

		break;
	case MessageFramer::FrameRequest:

		 // msg = new Protocol::FrameRequest();
		break;
	default:
		break;
	}
	delete msg.message;

}

void DicomStream::preFetch_()
{

	printf("starting prefetch thread\n");
	string file;
	while(!stopPrecache)
	{
		if (precacheQueue.wait_and_pop(file))
		    printf("prefetching file %s\n",file.c_str());
		//posix_fadvise();

	}

	printf("exiting prefetch thread\n");

}



void DicomStream::clientTest_()
{
	for (int i = 0; i < 100; ++i)
	{
		string host = "localhost";
	    int sockfd, n;
	    struct sockaddr_in serv_addr;

	    char buffer[256];
	    sockfd = socket(AF_INET, SOCK_STREAM, 0);
	    if (sockfd < 0)
	        perror("ERROR opening socket");

	    /* Disable the Nagle (TCP No Delay) algorithm */
	    int flag = 1;
	    int ret = setsockopt( sockfd, IPPROTO_TCP, TCP_NODELAY, (char *)&flag, sizeof(flag) );
	    if (ret == -1) {
	      printf("Couldn't setsockopt(TCP_NODELAY)\n");
	      exit( EXIT_FAILURE );
	    }

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
	    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
	    	perror("ERROR connecting");

	    Protocol::FrameGroupRequest* req = new Protocol::FrameGroupRequest();
	    req->set_studyuid("study1");
	    req->set_studyuidnumber(0);
	    req->set_seriesuid("series1");
	    req->set_seriesuidnumber(0);
	    req->set_type(Protocol::FrameGroupRequest_RequestType_Fetch);
	    req->set_priority(Protocol::FrameGroupRequest_Priority_Selected);
	    req->set_instanceuidprefix("");

	    Protocol::FrameRequest* frameReq = new Protocol::FrameRequest();
	    frameReq->set_instanceuid("instance1");
	    frameReq->set_instanceuidnumber(0);
	    frameReq->set_framenumber(1);
	    req->mutable_frames()->AddAllocated(frameReq);

	    frameReq = new Protocol::FrameRequest();
		frameReq->set_instanceuid("instance2");
		frameReq->set_instanceuidnumber(1);
		frameReq->set_framenumber(1);
		req->mutable_frames()->AddAllocated(frameReq);

		frameReq = new Protocol::FrameRequest();
		frameReq->set_instanceuid("instance3");
		frameReq->set_instanceuidnumber(2);
		frameReq->set_framenumber(1);
		req->mutable_frames()->AddAllocated(frameReq);
		req->set_multiframe(false);

	    MessageFramer* framer = new MessageFramer(sockfd);
	    framer->write(req);
	    delete framer;

	    bzero(buffer,256);
	    while ( (n = read(sockfd,buffer,255)) > 0 )
	    {
	        printf("%s\n",buffer);
	    }
	    if (n < 0)
	    	perror("ERROR reading from socket");

	    close(sockfd);
		sleep(1);

	}


}

int DicomStream::acquire(string fileName)
{
	TFileInfo* info = NULL;
	if (fileInfo.find(fileName) != fileInfo.end())
	{
		info = new TFileInfo();
	}
	else
	{
		info = fileInfo[fileName];
	}
	info->refCount++;
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
	return info->refCount;

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

void DicomStream::preFetch()
{
   Instance()->preFetch_();

}
void DicomStream::clientTest()
{

   Instance()->clientTest_();
}


/////////////////////////////////////////////






