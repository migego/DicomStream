/*
 * ImageIterator.h
 *
 *  Created on: Jun 28, 2011
 *      Author: aboxer
 */

#ifndef FRAMEGROUPITERATOR_H_
#define FRAMEGROUPITERATOR_H_

using namespace std;
#include "FrameIterator.h"
#include "../Iterators/UpDownIterator.h"
#include <string>


//iterator for either a series of single frame images, or a single multi-frame image
typedef UpDownIterator<Protocol::FrameFragment, FrameIterator> tFrameGroupIterator;

class FrameGroupIterator : public tFrameGroupIterator{
public:
	FrameGroupIterator(vector<FrameIterator*>* childIters, size_t primaryInd) : tFrameGroupIterator(childIters, primaryInd)
	{

	}

	FrameGroupIterator(string fName) : fileName(fName)
	{

	}
	FrameGroupIterator(vector<string> fNames)
	{
		if (fNames.empty())
			return;

		vector<string>::iterator iter;
		vector<FrameIterator*>* itms = new vector<FrameIterator*>();
		for (iter = fNames.begin(); iter != fNames.end(); ++iter)
		{
			itms->push_back(new FrameIterator(*iter));
		}
		setChildIterators(itms,0);

	}
	virtual ~FrameGroupIterator(){}


private:
	//for multi-frame image
	string fileName;
};

#endif /* FRAMEGROUPITERATOR_H_ */
