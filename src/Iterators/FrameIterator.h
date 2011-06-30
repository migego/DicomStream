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

// iterator for image frame
typedef SequentialIterator<Protocol::FrameFragment, FragmentIterator> tFrameIterator;


class FrameIterator : public tFrameIterator{
public:
	FrameIterator(vector<FragmentIterator*>* childIters, unsigned int fNumber) : tFrameIterator(childIters), frameNumber(fNumber){

	}
	FrameIterator(string fName) : fileName(fName)
	{

	}
	virtual ~FrameIterator(){

	}
private:
	string fileName;
	unsigned int frameNumber;
};

#endif /* FRAMEITERATOR_H_ */
