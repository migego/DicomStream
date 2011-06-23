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

typedef list<Protocol::FrameFragment*> FragmentList;
typedef map<int, FragmentList*> FrameFragments;

class FileParser {
public:
	FileParser();
	virtual ~FileParser();

	void parse(string fileName, int frameNumber);

	FragmentList* getFragments(int frameNumber);

private:
	int width;
	int height;
	FrameFragments frameFragments;

	string fileName;


};

#endif /* FILEPARSER_H_ */
