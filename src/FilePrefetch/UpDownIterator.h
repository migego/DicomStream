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
	vector<Iterator> items;
	size_t primaryIndex;
	size_t currentIndex;
	size_t count;
	bool done;
public:
	UpDownIterator(vector<Iterator> items, size_t primaryIndex) : items(items), primaryIndex(primaryIndex)
	{
		currentIndex= primaryIndex;
		count=0;
		done = false;

	}
	bool next(Data& item)
	{
		if (done())
			return false;

		//first time
		if (count == 0)
		{
			  nextFragment(item);
			  count = 2;
			  return true;
		}

		//is there another fragment ?
		if (nextFragment(item))
			return true;


		//try both directions
		for (int i = 0; i < 2; ++i)
		{
			count++;
			int incr = count >> 1;
			if ( count&1 == 1)
				incr *= -1;

			int nextIndex = primaryIndex + incr;
			if (nextIndex >= 0 && nextIndex < items.size())
			{
				currentIndex = nextIndex;
				return nextFragment(item);
			}
		}
		done = true;
		return false;

	}
	bool isDone()
	{
		return (items.empty() || done);
	}
protected:
	virtual bool nextFragment(Data& fragment)
	{
		Iterator iter  = items[currentIndex];
		bool rc =  iter.nextFragment(fragment);
		return rc;
	}

};

#endif /* UPDOWNITERATOR_H_ */
