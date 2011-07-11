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
		childIterators = NULL;
	}

	SequentialIterator(void)
	{
		setChildIterators(NULL);
	}

	void setChildIterators(vector<Iterator*>* childIters)
	{
		childIterators = childIters;
		position = 0;
	}

	bool next(Data& item)
	{
		if (!isValid())
			return false;
        while (!childIterators->operator[](position)->next(item))
        {
        	position++;
			if (isDone())
			{
				printf("[Server] SequentialIterator: done \n");
				finish();
				return false;
			}
        }
        return true;
	}

	void completeNext()
	{
		 if (!isValid() )
			   return;
		 if  (isInitialized() && !hasNext())
		 {
			 position = childIterators->size();
             finish();
			 printf("[Server] SequentialIterator: no next fragment: completed; done = %d, initialized = %d\n", isDone(), isInitialized());

		 }
	}

    bool isInitialized()
    {
    	return (childIterators != NULL);
    }
    bool isDone()
	 {
		return (position == childIterators->size());
	 }


private:
	virtual void finish()
	{

	}

	bool isValid()
	{
		return ( isInitialized() && !isDone() && !childIterators->empty());
	}

	bool hasNext()
	{
		//uninitialized iterater
		if (!isInitialized())
			return true;

		if (!isValid())
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
};

#endif /* SEQUENTIALITERATOR_H_ */
