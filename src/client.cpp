#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <boost/thread.hpp>
#include "client.h"
#include "Parse/stream.pb.h"
#include "Parse/MessageFramer.h"

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int runClient(string host, int portno)
{
    int sockfd, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    server = gethostbyname(host.c_str());
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
        error("ERROR connecting");

    Protocol::SeriesRequest* req = new Protocol::SeriesRequest();
    req->set_studyuid("studyuid");
    req->set_studyuidnumber(0);
    req->set_seriesuid("seriesuid");
    req->set_seriesuidnumber(0);
    req->set_type(Protocol::SeriesRequest_RequestType_Fetch);
    req->set_priority(Protocol::SeriesRequest_Priority_Selected);
    req->set_instanceuidprefix("");

    Protocol::FrameRequest* frameReq = new Protocol::FrameRequest();
    frameReq->set_instanceuid("instanceuid");
    frameReq->set_instanceuidnumber(0);
    frameReq->set_framenumber(1);


    MessageFramer* framer = new MessageFramer();
    framer->write(sockfd, req);
    delete framer;

    if (n < 0)
         error("ERROR writing to socket");
    bzero(buffer,256);
    n = read(sockfd,buffer,255);
    if (n < 0)
         error("ERROR reading from socket");
    printf("%s\n",buffer);
    close(sockfd);
    return 0;
}
