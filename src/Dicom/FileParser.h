/*
 * FileParser.h
 *
 *  Created on: May 26, 2011
 *      Author: aboxer
 */

#ifndef FILEPARSER_H_
#define FILEPARSER_H_


#include "imebra.h"

using namespace std;
using namespace puntoexe;
using namespace puntoexe::imebra;

#include <string>
#include "../Queue/FragmentIterator.h"

typedef map<int, FragIterVec*> FrameFragments;

class FileParser {
public:
	FileParser();
	virtual ~FileParser();

	void parse(string fileName, int frameNumber);

	FragIterVec* getFragments(int frameNumber);

	string getFileName(){ return fileName;}

private:
	imbxUint32 imageSizeX;
	imbxUint32 imageSizeY;
	image::bitDepth depth;
	std::wstring colorSpace;
	std::wstring transferSyntax;
	imbxUint8 channelsNumber;
	bool bInterleaved;
	bool b2Complement;
	imbxUint8 allocatedBits;
	imbxUint8 storedBits;
	imbxUint8 highBit;
	bool bSubSampledY;
	bool bSubSampledX;

	FrameFragments frameFragments;

	string fileName;

};

#endif /* FILEPARSER_H_ */
