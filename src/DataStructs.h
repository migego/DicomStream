/*
 * DataStructs.h
 *
 *  Created on: 2011-07-25
 *      Author: aaron
 */

#ifndef DATASTRUCTS_H_
#define DATASTRUCTS_H_

#include "Iterators/FrameGroupIterator.h"

struct TClientData {
	TClientData() : socketFd(-1), frameGroup(NULL) {}
	TClientData(int fd, FrameGroupIterator* fGroup) : socketFd(fd), frameGroup(fGroup) {}
	TClientData(TClientData* that) : socketFd(that->socketFd), frameGroup(that->frameGroup) {}
	int socketFd;
    FrameGroupIterator* frameGroup;
};

struct TEioData : public TClientData
{
	TEioData() : offset(0), size(0) {}
	TEioData(int sockFd, FrameGroupIterator* fGroup) : TClientData(sockFd, fGroup),offset(0), size(0) {}
	TEioData(int sockFd, FrameGroupIterator* fGroup, int off, int sze) :
		                         TClientData(sockFd, fGroup),
		                         offset(off),
		                         size(sze) {}
	TEioData(TClientData* clientData, int off, int sze) : TClientData(clientData), offset(off), size(sze) {}
	TEioData(TClientData* clientData) : TClientData(clientData) {}
	unsigned int offset;
	unsigned int size;
};


#endif /* DATASTRUCTS_H_ */
