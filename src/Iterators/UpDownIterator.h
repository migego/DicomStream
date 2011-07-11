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
	UpDownIterator(vector<Iterator*>* childIters, size_t primaryInd) : childIterators(childIters), primaryIndex(primaryInd), currentIndex(primaryInd), count(0), completedIterators(0)
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
    	  childIterators = NULL;
      }
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
			advanceCurrentIndex();
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

private:
	bool advanceCurrentIndex()
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
			if (nextIndex >= 0 && nextIndex < (int)childIterators->size())
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

	virtual void finish()
	{

	}

};

#endif /* UPDOWNITERATOR_H_ */
