/*
 * UpDownIterator.h
 *
 *  Created on: May 23, 2011
 *      Author: aboxer
 */

#ifndef SEQUENTIALITERATOR_H_
#define SEQUENTIALITERATOR_H_

using namespace std;
#include <vector>

template <typename Data, typename Iterator> class SequentialIterator {
private:
	vector<Iterator*>* childIterators;
	size_t position;
	bool done;
public:
	virtual ~SequentialIterator()
	{

	}

	SequentialIterator(void) : position(0), done(false)
	{
		setChildIterators(NULL);
	}

	void setChildIterators(vector<Iterator*>* childIters)
	{
		childIterators = childIters;
	}

	bool next(Data& item)
	{
		if ( childIterators == NULL || childIterators->empty() )
			return false;
        while (!childIterators->operator[](position)->next(item))
        {
        	position++;
            if (position == childIterators->size())
            {
            	done = true;
            	finish();
            	return false;
            }
        }

        return true;
	}

	bool isDone()
	{
		return done;
	}

private:
	virtual void finish()
	{

	}

};

#endif /* SEQUENTIALITERATOR_H_ */
