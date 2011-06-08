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
	MessageFramer() :  hasType(false), hasSize(false),data(NULL),offset(0)
	{

	}
	virtual ~MessageFramer()
	{

	}

	enum MessageType
	{
		None,
		SeriesRequest,
		FrameRequest,
		FrameFragment
	};

	struct GenericMessage
	{
		GenericMessage()
		{
			type = None;
			message = NULL;
		}
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

	void initRead()
	{
		hasType = false;
		hasSize = false;
		if (data)
			delete[] data;
		data = NULL;
		offset = 0;
	}

	GenericMessage read(int fd)
	{
		int n = 0;
		GenericMessage rc;

		if (!hasType)
		{
			n = ::read(fd, &type, 1);
			if (n < 0)
				error("Error reading from socket");
			hasType = true;
		}
		if (!hasSize)
		{
			n = ::read(fd, &size, 4);
			if (n < 0)
				error("Error reading from socket");
			hasSize = true;
			size = ntohl(size);
		}
		if (!data)
		    data = new char[size];

		if (offset != size)
		{
			n = ::read(fd, data+offset, size-offset);
			if (n < 0)
				error("Error reading from socket");
			offset += n;
		}


		if (offset == size)
		{
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

			rc.message = msg;
			rc.type = (MessageType)type;

		}
		return rc;
	}

private:
	bool hasType;
	char type;
	bool hasSize;
	int size;
	char* data;
	int offset;


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
