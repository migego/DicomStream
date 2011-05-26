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
#include <fcntl.h>
#include <errno.h>
#include <err.h>
#include <stddef.h>
#include <ev.h>
#include <sys/mount.h>

#include <boost/thread.hpp>
#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/info_parser.hpp>
#include <string>

#include "FilePrefetch/concurrent_queue.h"
#include "FilePrefetch/UpDownIterator.h"
#include "FilePrefetch/SimpleFragmentIterator.h"

struct client {
        int fd;
        ev_io ev_write;
		ev_io ev_read;
};

ev_io ev_accept;

int setnonblock(int fd)
{
    int flags = fcntl(fd, F_GETFL);
    if (flags < 0)
            return flags;
    flags |= O_NONBLOCK;
    if (fcntl(fd, F_SETFL, flags) < 0)
            return -1;

    return 0;
}

static void write_cb(struct ev_loop *loop, struct ev_io *w, int revents)
{
    client *cli= ((struct client*) (((char*)w) - offsetof(struct client,ev_write)));
	char superjared[]="HTTP/1.1 200 OK\r\nContent-Length: 336\r\nConnection: close\r\nContent-Type: text/html\r\nDate: Sat, 26 Apr 2008 01:13:35 GMT\r\nServer: lighttz/0.1\r\n\r\n<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.1//EN\" \"http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd\"><html xmlns=\"http://www.w3.org/1999/xhtml\" version=\"-//W3C//DTD XHTML 1.1//EN\" xml:lang=\"en\"><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"/><title>Hello World</title></head><body><p>Hello World</p></body></html>";

 	if (revents & EV_WRITE){
		write(cli->fd,superjared,strlen(superjared));
		ev_io_stop(EV_A_ w);
	}
 	close(cli->fd);
	delete cli;

}
static void read_cb(struct ev_loop *loop, struct ev_io *w, int revents)
{

    client *cli= ((client*) (((char*)w) - offsetof(client,ev_read)));
	int r=0;
	char rbuff[1024];
	if (revents & EV_READ){
		r=read(cli->fd,&rbuff,1024);
	}
	ev_io_stop(EV_A_ w);
	ev_io_init(&cli->ev_write,write_cb,cli->fd,EV_WRITE);
	ev_io_start(loop,&cli->ev_write);

}
static void accept_cb(struct ev_loop *loop, struct ev_io *w, int revents)
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


concurrent_queue< string, UpDownIterator< string, SimplFragmentIterator<string> > > precacheQueue;
bool stopPrecache = false;
void workerFunc()
{

	string file;
	if (precacheQueue.try_pop(file))
	{
		//posix_fadvise();
	}


}



int main()
{

	boost::property_tree::ptree pTree;
	try {
		read_info("src/DicomStream.cfg", pTree);
	}
	catch (boost::property_tree::info_parser_error &e) {
		std::cout << "error" << std::endl;
	}
	unsigned short port;
	try {
		port = pTree.get<int>("server.port");
	}

	catch(boost::property_tree::ptree_bad_path &e) {
		std::cout << "error" << std::endl;
	}

    boost::thread workerThread(workerFunc);


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
	ev_loop (loop, 0);

	workerThread.join();
	return 0;
}

