/*
 * IParseListener.h
 *
 *  Created on: Jun 29, 2011
 *      Author: aboxer
 */

#ifndef IPARSELISTENER_H_
#define IPARSELISTENER_H_

using namespace std;
#include <vector>
#include "../Iterators/FragmentIterator.h"


typedef vector<FragmentIterator*> tFragVec;

struct TParsedFrame
{
	TParsedFrame(tFragVec* fVec, int bytes) : fragVec(fVec), totalBytes(bytes)
	{
	}
	tFragVec* fragVec;
	int totalBytes;
};

class IParseListener {
public:
	virtual void parsed(vector< TParsedFrame* >& frameFragments)=0;
};

#endif /* IPARSELISTENER_H_ */
