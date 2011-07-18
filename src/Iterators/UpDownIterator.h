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
#include <stdio.h>

template <typename Data, typename Iterator> class UpDownIterator {

public:
	UpDownIterator(vector<Iterator*>* childIters, size_t primaryInd) : childIterators(childIters), primaryIndex(primaryInd), currentIndex(primaryInd), count(0), completedIterators(0), newPrimaryIndex(-1)
	{

	}

	virtual ~UpDownIterator()
	{
      if (childIterators != NULL)
      {
    	  typename vector<Iterator*>::iterator iter = childIterators->begin();
    	  while (iter != childIterators->end())
    	  {
    		  delete *iter;
    		  iter++;
    	  }
    	  delete childIterators;
      }
	}

	// this method won't actually change the primary index until the current iterator
	// is done
	void setPrimaryIndex(size_t primaryInd)
	{
		if (primaryInd == primaryIndex || !isValid() || primaryInd < 0 || primaryInd >= childIterators->size() )
			return;
		newPrimaryIndex = primaryInd;

	}

	// get next fragment; return false if parent iterator is not valid,
	// or we hit an uninitialized child iterator
	// current iterator is guaranteed to be valid after this operation completes
	bool next(Data& item)
	{
		 if (!isValid() )
			   return false;

        return currentIterator()->next(item);
	}

	// remove initialized current iterator that has no next fragment
	void completeNext()
	{
		 if (!isValid() )
			   return;
		 Iterator* iter = currentIterator();
		 iter->completeNext();
		 if (iter->isInitialized() && iter->isDone())
		 {
			printf("[Server] UpDownIterator: completed next\n");
			delete iter;
			childIterators->operator[](currentIndex) = NULL;
			completedIterators++;
			if (newPrimaryIndex != -1)
				updateIndex();
			else
			   advanceIndex();
		 }
	}

    bool isDone()
    {
    	return (completedIterators == childIterators->size());
    }
	Iterator* currentIterator()
	{
		if (!isValid())
			return NULL;

		return childIterators->operator[](currentIndex);
	}
protected:
	vector<Iterator*>* childIterators;
	size_t primaryIndex;
	size_t currentIndex;
	size_t count;
	size_t completedIterators;
	int newPrimaryIndex;

private:
	void updateIndex()
	{
		if (newPrimaryIndex == -1 || !isValid() )
			return;
		primaryIndex = newPrimaryIndex;
		currentIndex = newPrimaryIndex;
		count = 0;
		while (!currentIterator())
		{
			advanceIndex();
		}
		newPrimaryIndex = -1;
	}
	bool advanceIndex()
	{
		for (int i = 0; i < 2; ++i)
		{
			//we only count the primaryIndex once
			if (count == 0)
				  count=2;
			else
				count++;

			int incr = count >> 1;
			//if count is odd..
			if ( (count&1) == 1)
				incr *= -1;

			int nextIndex = primaryIndex + incr;
			// prospective index must be valid,
			// and child iterator at that index
			// must be non-null
			if (nextIndex >= 0 && nextIndex < (int)childIterators->size() && childIterators->operator[](nextIndex))
			{
				currentIndex = nextIndex;
				return true;
			}
		}
		return false;
	}

    bool isValid()
    {
		 return (!childIterators->empty() && !isDone() &&
				          currentIndex >= 0 &&  currentIndex < childIterators->size() );
    }

	virtual void finish() = 0;
};

#endif /* UPDOWNITERATOR_H_ */
