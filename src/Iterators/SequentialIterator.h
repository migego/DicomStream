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
public:
	virtual ~SequentialIterator()
	{
		if (childIterators != NULL)
		{
			typename vector<Iterator*>::iterator  iter;
			for (iter = childIterators->begin(); iter != childIterators->end(); iter++)
			{
				delete *iter;
			}
			delete childIterators;
		}
	}

	SequentialIterator(vector<Iterator*>* childIters)
	{
		setChildIterators(childIters);
	}

	SequentialIterator(void)
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
        while (!childIterators->back()->next(item))
        {
        	childIterators->pop_back();
            if (childIterators->empty())
            	return false;
        }
        return true;
	}

};

#endif /* SEQUENTIALITERATOR_H_ */
