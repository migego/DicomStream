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

public:
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

	void setChildIterators(vector<Iterator*>* childIters, size_t primaryInd)
	{
		childIterators = childIters;
		primaryIndex = primaryInd;
		currentIndex = primaryInd;
	}

	// get next fragment; return false if parent iterator is not valid,
	// or we hit an uninitialized child iterator
	// current iterator is guaranteed to be valid after this operation completes
	bool next(Data& item)
	{
		 if (!isValid() )
			   return false;

		//advance current index if current iterator is done
		while (!currentFragment(item) )
		{
			 advanceCurrentIndex();
			 if (isDone())
				 return false;
		}
		return true;


	}

	// remove current iterator that has no next fragment
	void completeNext()
	{

		 if (!isValid() )
			   return;

		 if (!currentIterator()->hasNext())
		 {
			 Data dummy;
			 next(dummy);
			 printf("[Server] remove iterator with no next fragment\n");
			 advanceCurrentIndex();
		 }
	}
    bool isInitialized()
    {
    	return (childIterators != NULL);
    }
    bool isDone()
    {
    	return (completedIterators == childIterators->size());
    }

protected:
	vector<Iterator*>* childIterators;
	size_t primaryIndex;
	size_t currentIndex;
	size_t count;
	size_t completedIterators;
	Iterator* currentIterator()
	{
		if (!isValid())
			return NULL;

		return childIterators->operator[](currentIndex);

	}
private:
    bool currentFragment(Data& fragment)
	{
    	Iterator* iter  = childIterators->operator[](currentIndex);
    	if (!iter)
    		return false;
		bool rc =  iter->next(fragment);
		//delete completed child iterator
		if (!rc && iter->isInitialized())
		{
			delete iter;
			childIterators->operator[](currentIndex) = NULL;
			completedIterators++;
		}
		return rc;
	}
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
			if ( count&1 == 1)
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
		 return (isInitialized() && !childIterators->empty() && !isDone() &&
				          currentIndex >= 0 &&  currentIndex < childIterators->size() );
    }

	virtual void finish()
	{

	}

};

#endif /* UPDOWNITERATOR_H_ */
