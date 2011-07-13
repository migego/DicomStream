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

class FragmentIterator  {

private:
	bool done;
	size_t offset;
	size_t size;
	size_t chunk;
	size_t offsetIncrement;

public:

	FragmentIterator(size_t offset, size_t size, size_t chunk) : offset(offset), size(size), chunk(chunk), offsetIncrement(0)
	{
	}
	void reset()
	{
		offsetIncrement = 0;
	}
    bool next( Protocol::FrameFragment& fragment)
	{
    	if (offsetIncrement == size)
    		return false;
		size_t block;
		if (chunk <=0)
			block = size;
		else
		   block = min(chunk, size-offsetIncrement);
		fragment.set_offset( offset + offsetIncrement);
		fragment.set_size(block);
		offsetIncrement += block;
		return true;
	}
    bool hasNext()
    {
    	return offsetIncrement < size;
    }

};

#endif /* FRAGMENTITERATOR_H_ */
