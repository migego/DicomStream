/*
 * FragmentIterator.h
 *
 *  Created on: May 24, 2011
 *      Author: aboxer
 */

#ifndef SIMPLEFRAGMENTITERATOR_H_
#define SIMPLEFRAGMENTITERATOR_H_



template <typename Data> class SimpleFragmentIterator {

private:
	Data item;
	bool done;

public:

	SimpleFragmentIterator(Data item): item(item), done(false)
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
