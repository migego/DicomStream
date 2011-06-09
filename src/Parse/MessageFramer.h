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
	MessageFramer(int fd) : fd(fd), hasType(false), hasSize(false),data(NULL),offset(0)
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

	void write(Protocol::SeriesRequest* seriesRequest)
	{
		write(SeriesRequest, seriesRequest);
	}
	void write(Protocol::FrameResponse* frameResponse)
	{
		write(FrameRequest, frameResponse);
	}
	void write(Protocol::FrameFragment* frameFragment)
	{
		write(FrameFragment, frameFragment);
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

	GenericMessage read()
	{
		int n = 0;
		GenericMessage rc;

		if (!hasType)
		{
			n = ::read(fd, &type, 1);
			if (n < 0)
				perror("Error reading from socket");
			hasType = true;
		}
		if (!hasSize)
		{
			n = ::read(fd, &size, sizeof(int));
			if (n < 0)
				perror("Error reading from socket");
			hasSize = true;
			size = ntohl(size);
		}
		if (!data)
		    data = new char[size];

		if (offset != size)
		{
			n = ::read(fd, data+offset, size-offset);
			if (n < 0)
				perror("Error reading from socket");
			offset += n;
		}


		if (offset == size)
		{
			::google::protobuf::Message* msg = NULL;
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
			default:
				break;
			}
			if (msg != NULL)
			{
				msg->ParseFromArray(data, size);

				rc.message = msg;
				rc.type = (MessageType)type;

				//prepare for next message
				initRead();
			}

		}
		return rc;
	}

private:

	int fd;

	bool hasType;
	char type;
	bool hasSize;
	int size;
	char* data;
	int offset;


	void write(char type, ::google::protobuf::Message* msg)
	{
		int n = ::write(fd, &type,1);
		if (n < 0)
			perror("Error writing to socket");
		int size = msg->ByteSize();
		int sizeBE = htonl(size);
		n = ::write(fd, &sizeBE, sizeof(size));
		if (n < 0)
			perror("Error writing to socket");
		char* data = new char[size];
		msg->SerializeToArray(data, size);
		n = ::write(fd, data, size );
		if (n < 0)
			perror("Error writing to socket");
	    delete[] data;
	}


};

#endif /* MESSAGEFRAMER_H_ */
