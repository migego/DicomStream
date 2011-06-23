/*
 * FileParser.cpp
 *
 *  Created on: May 26, 2011
 *      Author: aboxer
 */

#include "FileParser.h"

#include "imebra.h"
#include <string>

using namespace std;
using namespace puntoexe;
using namespace puntoexe::imebra;

FileParser::FileParser() {
}

FileParser::~FileParser() {
    map<int, FragmentList*>::iterator  mapIter;
	for (mapIter = frameFragments.begin(); mapIter != frameFragments.end(); mapIter++)
	{
		if (mapIter->second != NULL)
		{

			FragmentList::iterator listIter;
			for (listIter = mapIter->second->begin(); listIter != mapIter->second->end(); listIter++)
			{

                delete *listIter;

			}

			delete mapIter->second;
		}

	}
}

FragmentList* FileParser::getFragments(int frameNumber)
{
	if (frameFragments.find(frameNumber) != frameFragments.end())
	{
		return frameFragments[frameNumber];
	}
	return NULL;

}

void FileParser::parse(string fileName, int frameNumber)
{
	if (!this->fileName.empty() && fileName.compare(this->fileName) != 0)
	{
		//log warning here
		return;
	}
	this->fileName = fileName;

	FragmentList* frags;

	//get fragment list
	if (frameFragments.find(frameNumber) != frameFragments.end())
	{
		frags = frameFragments[frameNumber];
	}
	else
	{
        frags = new FragmentList();
        frameFragments[frameNumber] = frags;
	}

	// Open the file containing the dicom dataset
	ptr<puntoexe::stream> inputStream(new puntoexe::stream);
	inputStream->openFile(fileName, std::ios_base::in);

	// Connect a stream reader to the dicom stream. Several stream reader
	//  can share the same stream
	ptr<puntoexe::streamReader> reader(new streamReader(inputStream));

	// Get a codec factory and let it use the right codec to create a dataset
	//  from the input stream
	ptr<codecs::codecFactory> codecsFactory(codecs::codecFactory::getCodecFactory());
	ptr<dataSet> loadedDataSet = codecsFactory->load(reader, 2048);

	//read width, height, etc......

	// parse frame fragments
	imbxUint32 frameCount = 0;
	imbxUint32 offset = 0;
	imbxUint32 length=0;

	printf("==== %s =====\n",fileName.c_str());
	while( loadedDataSet->getImageOffset(frameCount,offset,length) )
	{
		Protocol::FrameFragment* fragment = new Protocol::FrameFragment();
		fragment->set_offset(offset);
		fragment->set_size(length);
		frags->push_front(fragment);

		printf("frame = %d, offset = %d, length = %d\n",frameCount,offset,length);
		frameCount++;
	}
}
