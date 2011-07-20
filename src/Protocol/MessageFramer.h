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
#include <stdio.h>
#include <arpa/inet.h>

class MessageFramer {
public:
	MessageFramer(int fd) : fd(fd), readBuffer(NULL), writeBuffer(NULL)
	{
       initRead();
       initWrite();
	}
	virtual ~MessageFramer()
	{

	}

	enum MessageType
	{
		None,
		SetPriorityRequest,
		SetPrimaryIndexRequest,
		FrameGroupRequest,
		FrameResponse,
		FrameFragmentHeader
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

    bool IsReadInProgress()
    {
    	return readInProgress;
    }

    bool IsWriteInProgress()
    {
    	return writeInProgress;
    }
	void initRead()
	{
		readInProgress = false;
		readHasType = false;
		readSizeOffset = 0;
		if (readBuffer)
			delete[] readBuffer;
		readBuffer = NULL;
		readOffset = 0;
		readSize=0;
	}

	void initWrite()
	{
		writeInProgress = false;
		writeSentType = false;
		writeSizeOffset = 0;
		if (writeBuffer)
			delete[] writeBuffer;
		writeBuffer = NULL;
		writeOffset = 0;
		writeSize=0;
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


	bool read(MessageWrapper& wrapper)
	{
		readInProgress = true;
		if (!readHasType)
		{
			int n = ::read(fd, &readType, 1);
			validateRead(n);
			if (n == 0)
				return false;

			readHasType = true;
		}

		while (readSizeOffset < 4)
		{
			int n = ::read(fd, ((unsigned char*)readRawSize)+readSizeOffset, 4-readSizeOffset);
			validateRead(n);
			if (n == 0)
				return false;
			readSizeOffset += n;
		}
		readSize = *((unsigned int*)readRawSize);
		readSize = ntohl(readSize);

		try
		{
			if (!readBuffer)
				readBuffer = new unsigned char[readSize];
		}
		catch (std::bad_alloc& ba)
		{
			printf("MessageFramer:read caught bad alloc\n");
			return false;
		}

		while ( readOffset < readSize)
		{
			int n = ::read(fd, readBuffer+readOffset, readSize-readOffset);
			validateRead(n);
			if (n == 0)
				return false;
			readOffset += n;
		}


		::google::protobuf::Message* msg = NULL;
		switch(readType)
		{
		case SetPriorityRequest:
			msg = new Protocol::SetPriorityRequest();
			break;
		case SetPrimaryIndexRequest:
			msg = new Protocol::SetPrimaryIndexRequest();
			break;
		case FrameGroupRequest:
			msg = new Protocol::FrameGroupRequest();
			break;
		case FrameResponse:
			  msg = new Protocol::FrameResponse();
			break;
		case FrameFragmentHeader:
			  msg = new Protocol::FrameFragmentHeader();
			break;
		default:
			printf("MessageFramer: unidentified message\n");
			break;
		}
		if (msg != NULL)
		{
			msg->ParseFromArray(readBuffer, readSize);
			wrapper.message = msg;
			wrapper.type = (MessageType)readType;

		}
		//prepare for next message
		initRead();

		return (msg != NULL);

	}

	bool write(char type, ::google::protobuf::Message* msg)
	{
		writeInProgress = true;
		if (!writeSentType)
		{
			int n = ::write(fd, &type,1);
			validateWrite(n);
			if (n == 0)
				return false;
			writeSentType = true;
		}


        while (writeSizeOffset < sizeof(int))
        {
			writeSize = msg->ByteSize();
			unsigned int sizeBE = htonl(writeSize);
			int n = ::write(fd, ((unsigned char*)&sizeBE) + writeSizeOffset, sizeof(int)-writeSizeOffset);
			validateWrite(n);
			if (n == 0)
				return false;
			writeSizeOffset +=n;
        }

        if (!writeBuffer)
        {
			writeBuffer = new unsigned char[writeSize];

        }
		msg->SerializeToArray(writeBuffer, writeSize);
		while (writeOffset < writeSize)
		{
			int n = ::write(fd, writeBuffer+writeOffset, writeSize-writeOffset );
			validateWrite(n);
			if (n == 0)
				return false;
			writeOffset += n;
		}

		initWrite();
		return true;

	}

private:

	int fd;

	bool readInProgress;
	bool readHasType;
	unsigned char readType;
	unsigned int readSizeOffset;
	unsigned char readRawSize[4];
	unsigned int readSize;
	unsigned int readOffset;
	unsigned char* readBuffer;


	bool writeInProgress;
	bool writeSentType;
	unsigned char writeType;
	unsigned int writeSizeOffset;
	unsigned char writeRawSize[4];
	unsigned int writeSize;
	unsigned int writeOffset;
	unsigned char* writeBuffer;






};

#endif /* MESSAGEFRAMER_H_ */
