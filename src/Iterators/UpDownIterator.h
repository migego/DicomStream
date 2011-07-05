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

	UpDownIterator(void) : count(0), completedIterators(0)
	{
		setChildIterators(NULL,0);
	}

	void setChildIterators(vector<Iterator*>* childIters, size_t primaryInd)
	{
		childIterators = childIters;
		primaryIndex = primaryInd;
		currentIndex = primaryInd;
	}

	bool hasNext()
	{
		 if (childIterators == NULL ||
				 childIterators->empty() ||
				    (completedIterators == childIterators->size()) ||
				       (currentIndex >= childIterators->size())  )
			   return false;
		 typename vector<Iterator*>::const_iterator iter = childIterators->begin();
		 while(iter != childIterators->end())
		 {
			 Iterator* iterVal = *iter;
             if (iterVal->hasNext() || !iterVal->isInitialized() )
            	 return true;
			 iter++;
		 }
		 return false;

	}
	bool next(Data& item)
	{
		 if (childIterators == NULL ||
				 childIterators->empty() ||
				    completedIterators == childIterators->size() ||
				       currentIndex >= childIterators->size()  )
			   return false;

		//is there another fragment in current iterator ?
		if (currentFragment(item))
			return true;

		//first increment
		if (count == 0)
		{
			  //after first item is done, then increment count, so that it will be equal to 2 at next increment
			  count++;
		}

		int incr = 0;
		while ( completedIterators < childIterators->size())
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
					if (!currentFragment(item))
						continue;
					return true;
				}
			}
		}
		return false;

	}
    bool isInitialized()
    {
    	return (childIterators != NULL);
    }

protected:
	vector<Iterator*>* childIterators;
	size_t primaryIndex;
	size_t currentIndex;
	size_t count;
	size_t completedIterators;
	Iterator* currentIterator()
	{
		if (!childIterators ||
				childIterators->empty() ||
				 (completedIterators == childIterators->size()))
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
		if (!rc)
		{
			delete iter;
			childIterators->operator[](currentIndex) = NULL;
			completedIterators++;
		}
		return rc;
	}

	virtual void finish()
	{

	}

};

#endif /* UPDOWNITERATOR_H_ */
