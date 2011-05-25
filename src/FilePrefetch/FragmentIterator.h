/*
 * FragmentIterator.h
 *
 *  Created on: May 24, 2011
 *      Author: aboxer
 */

#ifndef FRAGMENTITERATOR_H_
#define FRAGMENTITERATOR_H_

#include "UpDownIterator.h"
#include "../Parse/protocol.h"

struct Foo
{

};

class FragmentIterator : UpDownIterator< FrameFrag> {

public:

	FragmentIterator(vector< FrameFrag> items, size_t primaryIndex) : UpDownIterator<FrameFrag>(items, primaryIndex)
	{

	}

private:
	int fragmentCount;

protected:
	virtual bool nextFragment( FrameFrag& fragment)
	{
		fragment = items[currentIndex];
		return true;
	}
};

#endif /* FRAGMENTITERATOR_H_ */
