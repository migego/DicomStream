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
	MessageFramer(int fd) : fd(fd), hasType(false), sizeOffset(0),data(NULL),offset(0)
	{

	}
	virtual ~MessageFramer()
	{

	}

	enum MessageType
	{
		None,
		FrameGroupRequest,
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

	void write(Protocol::FrameGroupRequest* frameGroupRequest)
	{
		write(FrameGroupRequest, frameGroupRequest);
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
		sizeOffset = 0;
		if (data)
			delete[] data;
		data = NULL;
		offset = 0;
	}

	bool validateRead(int n)
	{
		if (n < 0)
		{
			perror("Error reading from socket");
		   close (fd);
		   return false;
		}
		return true;

	}

	GenericMessage read()
	{
		int n = 0;
		GenericMessage rc;

		if (!hasType)
		{
			n = ::read(fd, &type, 1);
			if (!validateRead(n) || n == 0)
				return rc;

			hasType = true;
		}
		if (sizeOffset != 4)
		{
			n = ::read(fd, rawSize+sizeOffset, 4-sizeOffset);
			if (!validateRead(n) || n == 0)
				return rc;
			sizeOffset += n;
			while (n > 0)
			{
				n = ::read(fd, rawSize+sizeOffset, 4-sizeOffset);
				if (!validateRead(n))
					return rc;
				sizeOffset += n;
			}
			if (sizeOffset != 4)
				return rc;
			size = ntohl( *((int*)rawSize));
		}
		if (!data)
		    data = new char[size];

		if (offset != size)
		{
			n = ::read(fd, data+offset, size-offset);
			if (!validateRead(n) )
				return rc;

			offset += n;
			while ( n > 0)
			{
				n = ::read(fd, data+offset, size-offset);
				if (!validateRead(n) )
					return rc;

				offset += n;
			}

		}


		if (offset == size)
		{
			::google::protobuf::Message* msg = NULL;
			switch(type)
			{
			case FrameGroupRequest:
				msg = new Protocol::FrameGroupRequest();
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
				printf("parsed message\n");
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
	char rawSize[4];
	int size;
	int sizeOffset;
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
