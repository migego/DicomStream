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
    // return false if iterator is done ( in which case the iterator will be freed and removed from parent)
    bool next( Data& fragment)
	{
    	if (done)
    		return false;
		fragment = item;
		done = true;
		return true;
	}
};

#endif /* SIMPLEFRAGMENTITERATOR_H_ */
