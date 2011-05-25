/*
 * IRequestParser.h
 *
 *  Created on: May 23, 2011
 *      Author: aboxer
 */

#ifndef IREQUESTPARSER_H_
#define IREQUESTPARSER_H_

class IRequestParser {
protected:
	bool complete;
public:
	IRequestParser(){ complete = false;}
	virtual ~IRequestParser(){}

	virtual unsigned int parse(unsigned char* buffer, unsigned int offset, unsigned int size) = 0;
	bool isComplete() { return complete;}
};

#endif /* IREQUESTPARSER_H_ */
