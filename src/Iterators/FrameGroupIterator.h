/*
 * ImageIterator.h
 *
 *  Created on: Jun 28, 2011
 *      Author: aboxer
 */

#ifndef FRAMEGROUPITERATOR_H_
#define FRAMEGROUPITERATOR_H_

using namespace std;
#include <string>

#include "FrameIterator.h"
#include "../Iterators/UpDownIterator.h"
#include "../IFileRefCounter.h"
#include "RefCounter.h"
#include "../Dicom/ParseListenManager.h"

//iterator for either a series of single frame images, or a single multi-frame image
typedef UpDownIterator<Protocol::FrameFragment, FrameIterator> tFrameGroupIterator;




class FrameGroupIterator : public tFrameGroupIterator, RefCounter{
public:

	// series of images
	FrameGroupIterator(vector<FrameIterator*>* childIters, size_t primaryInd) : tFrameGroupIterator(childIters, primaryInd)
	{

	}
	virtual ~FrameGroupIterator()
	{
	}

private:
    void finish()
	{
       //  release();
	}
};

#endif /* FRAMEGROUPITERATOR_H_ */
