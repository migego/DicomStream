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
#include "../Iterators/FrameGroupIterator.h"
typedef vector<FragmentIterator*> tFragVec;

class IParseListener {
public:
	virtual void parsed(vector< tFragVec* >& frameFragments)=0;
};

#endif /* IPARSELISTENER_H_ */
