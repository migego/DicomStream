/*
 * DicomStream.cpp
 *
 *  Created on: Jun 6, 2011
 *      Author: aboxer
 */

using namespace std;

#include <sys/types.h>
#include <sys/socket.h>
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

#include <sys/mount.h>

#include <boost/thread.hpp>
#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/info_parser.hpp>
#include <string>





#include "DicomStream.h"


DicomStream* DicomStream::instance=NULL;

DicomStream::DicomStream() {


}

DicomStream::~DicomStream() {
	// TODO Auto-generated destructor stub
}

////// STATIC ////////////////////////////
DicomStream* DicomStream::Instance()
{
	if (instance == NULL)
		instance = new DicomStream();
	return instance;

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
//////////////////////////////////////////

void DicomStream::start()
{
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

    boost::thread workerThread(preFetch);


	//create listen socket
    sockaddr_in listen_addr;
    int reuseaddr_on = 1;
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd < 0)
            err(1, "listen failed");
    if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &reuseaddr_on,
            sizeof(reuseaddr_on)) == -1)
            err(1, "setsockopt failed");
    memset(&listen_addr, 0, sizeof(listen_addr));
    listen_addr.sin_family = AF_INET;
    listen_addr.sin_addr.s_addr = INADDR_ANY;
    listen_addr.sin_port = htons(port);
    if (bind(listen_fd, (struct sockaddr *)&listen_addr,
            sizeof(listen_addr)) < 0)
            err(1, "bind failed");
    if (listen(listen_fd,5) < 0)
            err(1, "listen failed");
    if (setnonblock(listen_fd) < 0)
            err(1, "failed to set server socket to non-blocking");

    //listen to socket
	ev_io_init(&ev_accept,accept_cb,listen_fd,EV_READ);
    struct ev_loop *loop = ev_default_loop (0);
	ev_io_start(loop,&ev_accept);
    boost::thread clientTestThread(clientTest);

	ev_loop (loop, 0);
}

int DicomStream::setnonblock(int fd)
{
    int flags = fcntl(fd, F_GETFL);
    if (flags < 0)
            return flags;
    flags |= O_NONBLOCK;
    if (fcntl(fd, F_SETFL, flags) < 0)
            return -1;

    return 0;
}


void DicomStream::write_cb_(struct ev_loop *loop, struct ev_io *w, int revents)
{
    client *cli= ((client*) (((char*)w) - offsetof(struct client,ev_write)));
	char superjared[]="HTTP/1.1 200 OK\r\nContent-Length: 336\r\nConnection: close\r\nContent-Type: text/html\r\nDate: Sat, 26 Apr 2008 01:13:35 GMT\r\nServer: lighttz/0.1\r\n\r\n<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.1//EN\" \"http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd\"><html xmlns=\"http://www.w3.org/1999/xhtml\" version=\"-//W3C//DTD XHTML 1.1//EN\" xml:lang=\"en\"><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"/><title>Hello World</title></head><body><p>Hello World</p></body></html>";

 	if (revents & EV_WRITE){
		write(cli->fd,superjared,strlen(superjared));
		ev_io_stop(EV_A_ w);
	}
 	close(cli->fd);
	delete cli;

}


 void DicomStream::read_cb_(struct ev_loop *loop, struct ev_io *w, int revents)
{

    client *cli= ((client*) (((char*)w) - offsetof(client,ev_read)));
    MessageFramer::GenericMessage msg;
	if (revents & EV_READ){
		msg  = framer.read(cli->fd);

	}
	ev_io_stop(EV_A_ w);
	ev_io_init(&cli->ev_write,write_cb,cli->fd,EV_WRITE);
	ev_io_start(loop,&cli->ev_write);

}

void DicomStream::accept_cb_(struct ev_loop *loop, struct ev_io *w, int revents)
{
    sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
	int client_fd = accept(w->fd, (sockaddr *)&client_addr, &client_len);
	if (client_fd == -1) {
			return;
	}

    client* myClient = new client;
	myClient->fd=client_fd;
	if (setnonblock(myClient->fd) < 0)
         err(1, "failed to set client socket to non-blocking");
	ev_io_init(&myClient->ev_read,read_cb,myClient->fd,EV_READ);
	ev_io_start(loop,&myClient->ev_read);
}


void DicomStream::preFetch_()
{

	string file;
	if (precacheQueue.try_pop(file))
	{
		//posix_fadvise();
	}


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

	    Protocol::SeriesRequest* req = new Protocol::SeriesRequest();
	    req->set_studyuid("study1");
	    req->set_studyuidnumber(0);
	    req->set_seriesuid("series1");
	    req->set_seriesuidnumber(0);
	    req->set_type(Protocol::SeriesRequest_RequestType_Fetch);
	    req->set_priority(Protocol::SeriesRequest_Priority_Selected);
	    req->set_instanceuidprefix("");

	    Protocol::FrameRequest* frameReq = new Protocol::FrameRequest();
	    frameReq->set_instanceuid("US1_J2KR");
	    frameReq->set_instanceuidnumber(0);
	    frameReq->set_framenumber(1);


	    MessageFramer* framer = new MessageFramer();
	    framer->write(sockfd, req);
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




