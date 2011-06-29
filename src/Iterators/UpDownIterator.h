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
	vector<Iterator*>* items;
	size_t primaryIndex;
	size_t currentIndex;
	size_t count;
	bool done;
public:
	virtual ~UpDownIterator()
	{
		if (items != NULL)
		{
			typename vector<Iterator*>::iterator  iter;
			for (iter = items->begin(); iter != items->end(); iter++)
			{
				delete *iter;
			}
			delete items;
		}
	}

	UpDownIterator(vector<Iterator*>* itms, size_t primaryInd)
	{
		setItems(itms, primaryInd);
	}

	UpDownIterator(void)
	{
		setItems(NULL,0);
	}

	void setItems(vector<Iterator*>* itms, size_t primaryInd)
	{
		items = itms;
		primaryIndex = primaryInd;
		currentIndex = primaryInd;
		count = 0;
		done = false;
	}

	bool next(Data& item)
	{
		 if (done || items == NULL || currentIndex >= items->size() || items->empty() )
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

		int maxIncr = max(primaryIndex,  items->size() - primaryIndex)-1;
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
				if (nextIndex >= 0 && nextIndex < (int)items->size())
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
    	Iterator* iter  = items->operator[](currentIndex);
    	if (!iter)
    		return false;
		bool rc =  iter->next(fragment);
		if (!rc)
		{
			delete iter;
			items->operator[](currentIndex) = NULL;
		}
		return rc;
	}

};

#endif /* UPDOWNITERATOR_H_ */
