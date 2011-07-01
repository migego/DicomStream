/*
 * IFileRefCounter.h
 *
 *  Created on: Jun 29, 2011
 *      Author: aboxer
 */

#ifndef IFILEREFCOUNTER_H_
#define IFILEREFCOUNTER_H_

using namespace std;
#include <string>

class IFileRefCounter {
public:
	virtual int acquire(string fileName)=0;
	virtual int release(string fileName)=0;
	virtual int refCount(string fileName)=0;
};

#endif /* IFILEREFCOUNTER_H_ */
