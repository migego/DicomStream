/*
 * DicomPixels.cpp
 *
 *  Created on: May 26, 2011
 *      Author: aboxer
 */

#include "DicomPixels.h"
#include <string>
#include "unixStream.h"


DicomPixels::DicomPixels(int fd)
{

	if (fd == -1)
		return;


	// Open the file containing the dicom dataset
	ptr<puntoexe::unixStream> inputStream(new puntoexe::unixStream);
	inputStream->attach(fd);

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

	numberOfFrames = 1;
	if(!pData->getDataType(0x0028, 0, 0x0008).empty())
	{
		numberOfFrames = pData->getUnsignedLong(0x0028, 0, 0x0008, 0);
	}

	frameVec = new vector<FrameIterator*>();

	// for each frame, create a FrameIterator
	for (unsigned int i = 0; i < numberOfFrames; ++i)
	{

		imbxUint32 frameCount = 0;
		imbxUint32 offset = 0;
		imbxUint32 length=0;

		vector<FragmentIterator*>* fragVec = new vector<FragmentIterator*>();


		while( pData->getImageOffset(frameCount,offset,length) )
		{
			fragVec->push_back(new FragmentIterator(offset, length, -1));
			printf("frame = %d, offset = %d, length = %d\n",frameCount,offset,length);
			frameCount++;
		}
		frameVec->push_back(new FrameIterator(fragVec));

	}

}

DicomPixels::~DicomPixels()
{

}



