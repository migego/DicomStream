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
typedef UpDownIterator<TFrameFragment, FrameIterator> tFrameGroupIterator;




class FrameGroupIterator : public tFrameGroupIterator, RefCounter{
public:

	// series of images
	FrameGroupIterator(vector<FrameIterator*>* childIters, size_t primaryInd, string studyInstUid, string seriesInstUid) :
		     tFrameGroupIterator(childIters, primaryInd),
		     studyInstanceUid(studyInstUid),
		     seriesInstanceUid(seriesInstUid)
	{
	}

	virtual ~FrameGroupIterator(){}

	string  getStudyInstanceUid()
	{
		return studyInstanceUid;
	}
	string  getSeriesInstanceUid()
	{
		return seriesInstanceUid;
	}

private:
	string studyInstanceUid;
	string seriesInstanceUid;
    void finish()
	{
        release();
	}
};

#endif /* FRAMEGROUPITERATOR_H_ */
