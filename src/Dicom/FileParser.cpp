/*
 * FileParser.cpp
 *
 *  Created on: May 26, 2011
 *      Author: aboxer
 */

#include "FileParser.h"
#include <string>


FileParser::FileParser() {
}

FileParser::~FileParser() {
    map<int, FragIterVec*>::iterator  mapIter;
	for (mapIter = frameFragments.begin(); mapIter != frameFragments.end(); mapIter++)
	{
		if (mapIter->second != NULL)
		{
			FragIterVec::iterator listIter;
			for (listIter = mapIter->second->begin(); listIter != mapIter->second->end(); listIter++)
			{
                delete *listIter;

			}
			delete mapIter->second;
		}
	}
}

FragIterVec* FileParser::getFragments(int frameNumber)
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

	FragIterVec* frags;

	//get fragment list
	if (frameFragments.find(frameNumber) != frameFragments.end())
	{
		frags = frameFragments[frameNumber];
	}
	else
	{
        frags = new FragIterVec();
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
	ptr<dataSet> pData = codecsFactory->load(reader, 2048);

    transferSyntax = pData->getUnicodeString(0x0002, 0x0, 0x0010, 0x0);

	// Check for color space and sub-sampled channels
	///////////////////////////////////////////////////////////
	colorSpace=pData->getUnicodeString(0x0028, 0x0, 0x0004, 0x0);

	// Retrieve the number of planes
	///////////////////////////////////////////////////////////
	channelsNumber=(imbxUint8)pData->getUnsignedLong(0x0028, 0x0, 0x0002, 0x0);

	// Adjust the colorspace and the channels number for old
	//  NEMA files that don't specify those data
	///////////////////////////////////////////////////////////
	if(colorSpace.empty() && (channelsNumber == 0 || channelsNumber == 1))
	{
		colorSpace = L"MONOCHROME2";
		channelsNumber = 1;
	}

	if(colorSpace.empty() && channelsNumber == 3)
	{
		colorSpace = L"RGB";
	}

	// Retrieve the image's size
	///////////////////////////////////////////////////////////
    imageSizeX=pData->getUnsignedLong(0x0028, 0x0, 0x0011, 0x0);
    imageSizeY=pData->getUnsignedLong(0x0028, 0x0, 0x0010, 0x0);
	if((imageSizeX == 0) || (imageSizeY == 0))
	{
		//PUNTOEXE_THROW(codecExceptionCorruptedFile, "The size tags are not available");
	}

	// Check for interleaved planes.
	///////////////////////////////////////////////////////////
    bInterleaved = (pData->getUnsignedLong(0x0028, 0x0, 0x0006, 0x0)==0x0);

	// Check for 2's complement
	///////////////////////////////////////////////////////////
     b2Complement=pData->getUnsignedLong(0x0028, 0x0, 0x0103, 0x0)!=0x0;

	// Retrieve the allocated/stored/high bits
	///////////////////////////////////////////////////////////
    allocatedBits=(imbxUint8)pData->getUnsignedLong(0x0028, 0x0, 0x0100, 0x0);
    storedBits=(imbxUint8)pData->getUnsignedLong(0x0028, 0x0, 0x0101, 0x0);
    highBit=(imbxUint8)pData->getUnsignedLong(0x0028, 0x0, 0x0102, 0x0);
	if(highBit<storedBits-1)
		highBit=storedBits-1;


	// If the chrominance channels are sub-sampled, then find
	//  the right image's size
	///////////////////////////////////////////////////////////
    bSubSampledY=channelsNumber>0x1 && transforms::colorTransforms::colorTransformsFactory::isSubsampledY(colorSpace);
    bSubSampledX=channelsNumber>0x1 && transforms::colorTransforms::colorTransformsFactory::isSubsampledX(colorSpace);

	// get depth
	///////////////////////////////////////////////////////////
	if(b2Complement)
	{
		if(highBit >= 16)
		{
			depth = image::depthS32;
		}
		else if(highBit >= 8)
		{
			depth = image::depthS16;
		}
		else
		{
			depth = image::depthS8;
		}
	}
	else
	{
		if(highBit >= 16)
		{
			depth = image::depthU32;
		}
		else if(highBit >= 8)
		{
			depth = image::depthU16;
		}
		else
		{
			depth = image::depthU8;
		}
	}

	// parse frame fragments
	imbxUint32 frameCount = 0;
	imbxUint32 offset = 0;
	imbxUint32 length=0;

	printf("==== %s =====\n",fileName.c_str());
	while( pData->getImageOffset(frameCount,offset,length) )
	{
		FragmentIterator* iter = new FragmentIterator(offset, length, -1);
		frags->insert(frags->end(),iter);

		printf("frame = %d, offset = %d, length = %d\n",frameCount,offset,length);
		frameCount++;
	}
}
