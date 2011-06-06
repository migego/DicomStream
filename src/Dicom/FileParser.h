/*
 * FileParser.h
 *
 *  Created on: May 26, 2011
 *      Author: aboxer
 */

#ifndef FILEPARSER_H_
#define FILEPARSER_H_

using namespace std;
#include <string>
#include "../Parse/stream.pb.h"
#include <list>

class FileParser {
public:
	FileParser();
	virtual ~FileParser();

	void parse(string fileName);

private:
	int width;
	int height;
	std::list<Protocol::FrameFragment*> fragments;


};

#endif /* FILEPARSER_H_ */
