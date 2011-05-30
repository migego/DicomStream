/*
 * FragmentIterator.h
 *
 *  Created on: May 24, 2011
 *      Author: aboxer
 */

#ifndef SIMPLEFRAGMENTITERATOR_H_
#define SIMPLEFRAGMENTITERATOR_H_

#include "../Parse/protocol.h"


template <typename Data> class SimplFragmentIterator {

private:
	Data item;
	bool done;

public:

	SimplFragmentIterator(Data item): item(item), done(false)
	{

	}

    bool nextFragment( Data& fragment)
	{
    	if (done)
    		return false;
		fragment = item;
		done = true;
		return true;
	}
};

#endif /* SIMPLEFRAGMENTITERATOR_H_ */
