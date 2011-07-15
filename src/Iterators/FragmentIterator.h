/*
 * FragmentIterator.h
 *
 *  Created on: May 24, 2011
 *      Author: aboxer
 */

#ifndef FRAGMENTITERATOR_H_
#define FRAGMENTITERATOR_H_

#include <math.h>
#include <string>
#include "../Protocol/stream.pb.h"

struct TFrameFragment
{
	size_t offset;
	size_t size;  //size of fragment
};

class FragmentIterator  {

private:
	size_t offsetBase;
	size_t offsetIncrement;
	size_t size;
	size_t chunk;


public:

	FragmentIterator(size_t offset, size_t size, size_t chunk) : offsetBase(offset), offsetIncrement(0), size(size), chunk(chunk)
	{
	}
	FragmentIterator* copy()
	{
		return new FragmentIterator(offsetBase, size, chunk);
	}
    bool next( TFrameFragment& fragment)
	{
    	if (offsetIncrement == size)
    		return false;
		size_t block;
		if (chunk <=0)
			block = size;
		else
		   block = min(chunk, size-offsetIncrement);
		fragment.offset =  offsetBase + offsetIncrement;
		fragment.size =  block;
		offsetIncrement += block;
		return true;
	}
    bool hasNext()
    {
    	return offsetIncrement < size;
    }

};

#endif /* FRAGMENTITERATOR_H_ */
