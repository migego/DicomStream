/*
 * FragmentIterator.h
 *
 *  Created on: May 24, 2011
 *      Author: aboxer
 */

#ifndef FRAGMENTITERATOR_H_
#define FRAGMENTITERATOR_H_

#include <math.h>


class FragmentIterator  {

private:
	bool done;
	size_t offset;
	size_t size;
	size_t chunk;
	size_t offsetIncrement;

public:

	FragmentIterator(size_t offset, size_t size, size_t chunk) : done(false), offset(offset), size(size), chunk(chunk), offsetIncrement(0)
	{
	}
    bool next( FrameFragment& fragment)
	{
		if (done)
			return false;
		size_t block = min(chunk, size-offsetIncrement);
		fragment.offset = offset + offsetIncrement;
		fragment.size = block;
		offsetIncrement += block;
		if (offsetIncrement == size)
		{
			done = true;
		}
		return true;
	}
};

#endif /* FRAGMENTITERATOR_H_ */
