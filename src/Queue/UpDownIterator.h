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

template <typename Data, typename FragmentIterator> class UpDownIterator {
protected:
	vector<FragmentIterator*>* items;
	size_t primaryIndex;
	size_t currentIndex;
	size_t count;
	bool done;
public:
	virtual ~UpDownIterator()
	{
		if (items != NULL)
			delete items;
	}
	UpDownIterator(vector<FragmentIterator*>* itms, size_t primaryInd)
	{
		items = itms;
		primaryIndex = primaryInd;
		currentIndex = primaryInd;
		count=0;
		done=false;
	    if ( currentIndex >= items->size()  )
	    	done = true;
		if (items == NULL || items->empty())
			done = true;

	}


	bool nextFragment(Data& item)
	{
		if (done)
			return false;

		//is there another fragment ?
		if (getNextFragment(item))
			return true;

		//first time
		if (count == 0)
		{
			  //after first item is done, then increment count, so that it will be equal to 2 at next increment
			  count++;
		}



		int maxIncr = max(primaryIndex,  items->size() - primaryIndex)-1;
		int incr = 0;
		while (incr <= maxIncr)
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
				if (nextIndex >= 0 && nextIndex < items->size())
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
    bool getNextFragment(Data& fragment)
	{
    	FragmentIterator* iter  = items->operator[](currentIndex);
    	if (!iter)
    		return false;
		bool rc =  iter->nextFragment(fragment);
		if (!rc)
		{
			delete iter;
			items->operator[](currentIndex) = NULL;
		}
		return rc;
	}

};

#endif /* UPDOWNITERATOR_H_ */
