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

//iterator for either a series of single frame images, or a single multi-frame image
typedef UpDownIterator<Protocol::FrameFragment, FrameIterator> tFrameGroupIterator;

class FrameGroupIterator : public tFrameGroupIterator, RefCounter{
public:

	// series of images
	FrameGroupIterator(IFileRefCounter* refCounter, vector<string> fNames)
	{
		if (fNames.empty())
			return;

		vector<string>::iterator iter;
		vector<FrameIterator*>* itms = new vector<FrameIterator*>();
		for (iter = fNames.begin(); iter != fNames.end(); ++iter)
		{
			itms->push_back(new FrameIterator(refCounter, *iter));
		}
		setChildIterators(itms,0);

	}
	virtual ~FrameGroupIterator()
	{
		release();
	}

private:
    void finish()
	{
         release();
	}
};

#endif /* FRAMEGROUPITERATOR_H_ */
