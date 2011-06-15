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
	size_t size;
	size_t offset;
	size_t chunk;

public:

	FragmentIterator(size_t size, size_t chunk) : done(false), size(size), offset(0), chunk(chunk)
	{
	}
    bool next( FrameFragment& fragment)
	{
		if (done)
			return false;
		size_t size = min(chunk, size-offset);
		fragment.offset = offset;
		fragment.size = size;
		offset += size;
		if (offset == size)
		{
			done = true;
		}
		return true;
	}
};

#endif /* FRAGMENTITERATOR_H_ */
