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
	vector<Iterator*>* items;
public:
	virtual ~SequentialIterator()
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

	SequentialIterator(vector<Iterator*>* itms) : items(itms)
	{
	}

	bool next(Data& item)
	{
		if ( items == NULL || items->empty() )
			return false;
        while (!items->back()->next(item))
        {
        	items->pop_back();
            if (items->empty())
            	return false;
        }
        return true;
	}

};

#endif /* SEQUENTIALITERATOR_H_ */
