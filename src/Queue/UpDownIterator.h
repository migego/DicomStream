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
	vector<FragmentIterator*> items;
	size_t primaryIndex;
	size_t currentIndex;
	size_t count;
	bool done;
public:
	UpDownIterator(vector<FragmentIterator*> items, size_t primaryIndex) : items(items),
																primaryIndex(primaryIndex),
																currentIndex(primaryIndex)
	{
		count=0;
		done = false;
		if (items.empty() || currentIndex < 0 || currentIndex >= items.size())
			done = true;

	}
	bool next(Data& item)
	{
		if (done)
			return false;

		//first time
		if (count == 0)
		{
			  if (nextFragment(item))
			     return true;
			  //after first item is done, then increment count to 2
			  count = 2;
		}

		//is there another fragment ?
		if (nextFragment(item))
			return true;

		int maxIncr = max(primaryIndex, items.count - primaryIndex)-1;
		int incr = 0;
		while (incr <= maxIncr)
		{
			//try both directions
			for (int i = 0; i < 2; ++i)
			{
				count++;
			    incr = count >> 1;
				if ( count&1 == 1)
					incr *= -1;

				int nextIndex = primaryIndex + incr;
				if (nextIndex >= 0 && nextIndex < items.size())
				{
					currentIndex = nextIndex;
					if (!nextFragment(item))
						continue;
					return true;
				}
			}
		}
		done = true;
		return false;

	}
    bool nextFragment(Data& fragment)
	{
    	FragmentIterator* iter  = items[currentIndex];
		bool rc =  iter->nextFragment(fragment);
		if (!rc)
		{
			delete iter;
			items[currentIndex] = NULL;
		}
		return rc;
	}

};

#endif /* UPDOWNITERATOR_H_ */
