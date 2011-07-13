/*
 * FrameIterator.h
 *
 *  Created on: Jun 28, 2011
 *      Author: aboxer
 */

#ifndef FRAMEITERATOR_H_
#define FRAMEITERATOR_H_

#include "../Iterators/FragmentIterator.h"
#include "../Iterators/SequentialIterator.h"
#include "../Protocol/stream.pb.h"

#include "../IFileRefCounter.h"
#include "RefCounter.h"
#include "../Dicom/IParseListener.h"
#include "../Dicom/ParseListenManager.h"

// iterator for image frame
typedef SequentialIterator<Protocol::FrameFragment, FragmentIterator> tFrameIterator;

struct TFrameInfo
{
	TFrameInfo() : totalBytes(0), sentFrameHeader(false){}
	string fileName;
	Protocol::FrameRequest frameRequest;
	int totalBytes;
	bool sentFrameHeader;

};


class FrameIterator : public tFrameIterator, public RefCounter, public IParseListener{
public:

	FrameIterator(IFileRefCounter* refCounter, ParseListenManager* listenManager, TFrameInfo frameInfo) :
		                                                                 RefCounter(refCounter, frameInfo.fileName),
		                                                                 frameInfo(frameInfo),
	                                                                     frameNumber(0),
	                                                                     parseListenManager(listenManager)
	{
		if (parseListenManager)
			parseListenManager->addListener(frameInfo.fileName, this);
	}
	virtual ~FrameIterator(){
        finish();
	}

	void setChildIterators(vector<FragmentIterator*>* childIters)
	{
		tFrameIterator::setChildIterators(childIters);
	}

	void parsed(vector< TParsedFrame* >& frameFragments)
	{
       if (!isInitialized() && !frameFragments.empty())
       {
    	   setChildIterators(frameFragments[0]->fragVec);
    	   frameInfo.totalBytes = frameFragments[0]->totalBytes;

       }

	}
	TFrameInfo* getFrameInfo()
	{
		return &frameInfo;
	}

private:
	TFrameInfo frameInfo;
	unsigned int frameNumber;
	ParseListenManager* parseListenManager;

    void finish()
	{

    	if (!doneReleased())
    	{
			if (parseListenManager)
				parseListenManager->removeListener(frameInfo.fileName, this);
			//release();
    	}

	}
};

#endif /* FRAMEITERATOR_H_ */
