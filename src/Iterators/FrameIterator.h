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

// iterator for image frame
typedef SequentialIterator<Protocol::FrameFragment, FragmentIterator> tFrameIterator;


class FrameIterator : public tFrameIterator, RefCounter{
public:

	FrameIterator(IFileRefCounter* refCounter, string fName) : RefCounter(refCounter, fName)
	{
	}
	virtual ~FrameIterator(){

	}

	void setChildIterators(vector<FragmentIterator*>* childIters, unsigned int fNumber)
	{
		tFrameIterator::setChildIterators(childIters);
		frameNumber = fNumber;
	}

private:
	unsigned int frameNumber;

    void finish()
	{
        release();
	}
};

#endif /* FRAMEITERATOR_H_ */
