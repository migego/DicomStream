/*
 * UpDownIterator.h
 *
 *  Created on: May 23, 2011
 *      Author: aboxer
 */

#ifndef UPDOWNITERATOR_H_
#define UPDOWNITERATOR_H_

using namespace std;
#include <vector>

template <typename Data, typename Iterator> class UpDownIterator {
protected:
	vector<Iterator*>* childIterators;
	size_t primaryIndex;
	size_t currentIndex;
	size_t count;
	bool done;
public:
	virtual ~UpDownIterator()
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

	UpDownIterator(void)
	{
		setChildIterators(NULL,0);
	}

	void setChildIterators(vector<Iterator*>* childIters, size_t primaryInd)
	{
		childIterators = childIters;
		primaryIndex = primaryInd;
		currentIndex = primaryInd;
		count = 0;
		done = false;
	}

	bool next(Data& item)
	{
		 if (done || childIterators == NULL || currentIndex >= childIterators->size() || childIterators->empty() )
			   return false;

		//is there another fragment in current iterator ?
		if (getNextFragment(item))
			return true;

		//first increment
		if (count == 0)
		{
			  //after first item is done, then increment count, so that it will be equal to 2 at next increment
			  count++;
		}

		int maxIncr = max(primaryIndex,  childIterators->size() - primaryIndex)-1;
		int incr = 0;
		while ( abs(incr) <= maxIncr)
		{
			//try both directions
			for (int i = 0; i < 2; ++i)
			{
				count++;
			    incr = count >> 1;
			    //if count is odd..
				if ( count&1 == 1)
					incr *= -1;

				int nextIndex = primaryIndex + incr;
				if (nextIndex >= 0 && nextIndex < (int)childIterators->size())
				{
					currentIndex = nextIndex;
					if (!getNextFragment(item))
						continue;
					return true;
				}
			}
		}
		done = true;
		return false;

	}
private:
    bool getNextFragment(Data& fragment)
	{
    	Iterator* iter  = childIterators->operator[](currentIndex);
    	if (!iter)
    		return false;
		bool rc =  iter->next(fragment);
		if (!rc)
		{
			delete iter;
			childIterators->operator[](currentIndex) = NULL;
		}
		return rc;
	}

	virtual void finish()
	{

	}

};

#endif /* UPDOWNITERATOR_H_ */
