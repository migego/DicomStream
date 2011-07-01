/*
 * RefCounter.h
 *
 *  Created on: Jun 30, 2011
 *      Author: aboxer
 */

#ifndef REFCOUNTER_H_
#define REFCOUNTER_H_

#include <string>
#include "../IFileRefCounter.h"

class RefCounter {
public:
	RefCounter(IFileRefCounter* refCounter, string fName): fileName(fName),
	                                                       fileRefCounter(refCounter),
	                                                       releasedRefCount(false)
	{
		if (refCounter)
		   refCounter->acquire(fName);
	}
	RefCounter() : fileName(""), fileRefCounter(NULL), releasedRefCount(false)
	{

	}

	virtual ~RefCounter()
	{
		release();
	}
	string getFileName()
	{
		return fileName;
	}
protected:
	void release()
	{
		if (!releasedRefCount && fileRefCounter)
			fileRefCounter->release(fileName);
		releasedRefCount = true;

	}
	bool doneReleased()
	{
		return releasedRefCount;
	}
private:
	string fileName;
	IFileRefCounter* fileRefCounter;
	bool releasedRefCount;
};

#endif /* REFCOUNTER_H_ */
