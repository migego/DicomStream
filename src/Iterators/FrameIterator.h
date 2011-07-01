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


class FrameIterator : public tFrameIterator, public RefCounter, public IParseListener{
public:

	FrameIterator(IFileRefCounter* refCounter, ParseListenManager* listenManager, string fName) :
		                                                                 RefCounter(refCounter, fName),
	                                                                     frameNumber(0),
	                                                                     parseListenManager(listenManager)
	{
		if (parseListenManager)
			parseListenManager->addListener(fName, this);
	}
	virtual ~FrameIterator(){
        finish();
	}

	void setChildIterators(vector<FragmentIterator*>* childIters)
	{
		tFrameIterator::setChildIterators(childIters);
	}

	void parsed(vector< tFragVec* >& frameFragments)
	{
       if (!frameFragments.empty())
    	   setChildIterators(frameFragments[0]);
	}

private:
	unsigned int frameNumber;
	ParseListenManager* parseListenManager;

    void finish()
	{
    	if (!doneReleased())
    	{
			if (parseListenManager)
				parseListenManager->removeListener(getFileName(), this);
			release();
    	}
	}
};

#endif /* FRAMEITERATOR_H_ */
