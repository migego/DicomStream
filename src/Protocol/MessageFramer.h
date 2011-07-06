/*
 * MessageWrapper.h
 *
 *  Created on: Jun 5, 2011
 *      Author: aboxer
 */

#ifndef MESSAGEFRAMER_H_
#define MESSAGEFRAMER_H_

#include "stream.pb.h"
#include "ReadException.h"
#include "EAgainException.h"
#include "WriteException.h"

#include <errno.h>
#include <err.h>
#include <stddef.h>

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
		FrameResponse,
		FrameFragment
	};

	struct MessageWrapper
	{
		MessageWrapper()
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
		write(FrameResponse, frameResponse);
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

	void validateRead(int n)
	{
		if (n < 0)
		{
			//EAGAIN is acceptable on non-blocking socket; just means that read
			//buffer is empty
			if (errno == EAGAIN)
			{
				printf("[server] EAGAIN when reading from socket\n");
				throw EAgainException();
			}
			throw ReadException();
		}
	}
	void validateWrite(int n)
	{
		if (n < 0)
		{
			//EAGAIN is acceptable on non-blocking socket; just means that write
			//buffer is full
			if (errno == EAGAIN)
			{
				printf("[server] EAGAIN when writing to socket\n");
				throw EAgainException();
			}
			throw WriteException();
		}
	}

    void read(MessageWrapper& wrapper)
    {
    	while(!read_(wrapper))
    	{
    	}
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

	bool read_(MessageWrapper& wrapper)
	{
		int n = 0;

		if (!hasType)
		{
			n = ::read(fd, &type, 1);
			validateRead(n);
			if (n == 0)
				return false;

			hasType = true;
		}
		if (sizeOffset != 4)
		{
			n = ::read(fd, rawSize+sizeOffset, 4-sizeOffset);
			validateRead(n);
			if (n == 0)
				return false;
			sizeOffset += n;
			while (sizeOffset != 4 && n > 0)
			{
				n = ::read(fd, rawSize+sizeOffset, 4-sizeOffset);
				validateRead(n);
				sizeOffset += n;
			}
			if (sizeOffset != 4)
				return false;
			size = ntohl( *((int*)rawSize));
		}
		if (!data)
		    data = new char[size];

		if (offset != size)
		{
			n = ::read(fd, data+offset, size-offset);
			validateRead(n);
			if (n == 0)
				return false;

			offset += n;
			while ( offset != size && n > 0)
			{
				n = ::read(fd, data+offset, size-offset);
				validateRead(n);
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
			case FrameResponse:
				  msg = new Protocol::FrameResponse();
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
				wrapper.message = msg;
				wrapper.type = (MessageType)type;

				//prepare for next message
				initRead();

				return true;
			}

		}
		return false;
	}

	void write(char type, ::google::protobuf::Message* msg)
	{
		int n = ::write(fd, &type,1);
		validateWrite(n);
		int size = msg->ByteSize();
		int sizeBE = htonl(size);
		n = ::write(fd, &sizeBE, sizeof(size));
		validateWrite(n);
		char* data = new char[size];
		msg->SerializeToArray(data, size);
		n = ::write(fd, data, size );
		delete[] data;
		validateWrite(n);

	}


};

#endif /* MESSAGEFRAMER_H_ */
