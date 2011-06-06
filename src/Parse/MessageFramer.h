/*
 * MessageWrapper.h
 *
 *  Created on: Jun 5, 2011
 *      Author: aboxer
 */

#ifndef MESSAGEFRAMER_H_
#define MESSAGEFRAMER_H_

#include "stream.pb.h"

class MessageFramer {
public:
	virtual ~MessageFramer()
	{

	}

	enum MessageType
	{
		SeriesRequest,
		FrameRequest,
		FrameFragment
	};

	struct GenericMessage
	{
		MessageType type;
		::google::protobuf::Message* message;


	};

	void write(int fd, Protocol::SeriesRequest* seriesRequest)
	{
		write(fd, SeriesRequest, seriesRequest);
	}
	void write(int fd, Protocol::FrameResponse* frameResponse)
	{
		write(fd, FrameRequest, frameResponse);
	}
	void write(int fd, Protocol::FrameFragment* frameFragment)
	{
		write(fd, FrameFragment, frameFragment);
	}

	GenericMessage read(int fd)
	{
		char type;
		int n = ::read(fd, &type, 1);
		if (n < 0)
			error("Error reading from socket");

		int size;
		n = ::read(fd, &size, 4);
		if (n < 0)
			error("Error reading from socket");
		size = ntohl(size);
		char* data = new char[size];
		n = ::read(fd, data, size);
		if (n < 0)
			error("Error reading from socket");

		::google::protobuf::Message* msg;
		switch(type)
		{
		case SeriesRequest:
            msg = new Protocol::SeriesRequest();
			break;
		case FrameRequest:
			  msg = new Protocol::FrameRequest();
			break;
		case FrameFragment:
			  msg = new Protocol::FrameFragment();
			break;
		}
	    msg->ParseFromArray(data, size);
	    GenericMessage rc;
	    rc.message = msg;
	    rc.type = (MessageType)type;
		return rc;
	}

private:
	void write(int fd, char type, ::google::protobuf::Message* msg)
	{
		int n = ::write(fd, &type,1);
		if (n < 0)
			error("Error writing to socket");
		int size = msg->ByteSize();
		int sizeBE = htonl(size);
		n = ::write(fd, &sizeBE, sizeof(size));
		if (n < 0)
			error("Error writing to socket");
		char* data = new char[size];
		msg->SerializeToArray(data, size);
		n = ::write(fd, data, size );
		if (n < 0)
			error("Error writing to socket");
	    delete[] data;
	}
	void error(const char *msg)
	{
	    perror(msg);
	}

};

#endif /* MESSAGEFRAMER_H_ */
