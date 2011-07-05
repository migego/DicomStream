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
public:
	virtual ~SequentialIterator()
	{

	}

	SequentialIterator(void) : position(0)
	{
		setChildIterators(NULL);
	}

	void setChildIterators(vector<Iterator*>* childIters)
	{
		childIterators = childIters;
	}

	bool next(Data& item)
	{
		if ( childIterators == NULL || childIterators->empty() || (position == childIterators->size()))
			return false;
        while (!childIterators->operator[](position)->next(item))
        {
        	position++;
            if (position == childIterators->size())
            {
            	finish();
            	return false;
            }
        }

        return true;
	}
	bool hasNext()
	{
		if ( childIterators == NULL || childIterators->empty() || (position == childIterators->size()))
			return false;
		size_t pos = position;
		while (!childIterators->operator[](pos)->hasNext() )
		{
			pos++;
			if (pos == childIterators->size())
				return false;
		}
		return true;
	}

    bool isInitialized()
    {
    	return (childIterators != NULL);
    }

private:
	virtual void finish()
	{

	}

};

#endif /* SEQUENTIALITERATOR_H_ */
