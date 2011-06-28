/*
 * DicomPixels.h
 *
 *  Created on: May 26, 2011
 *      Author: aboxer
 */

#ifndef DICOMPIXELS_H_
#define DICOMPIXELS_H_


#include "imebra.h"

using namespace std;
using namespace puntoexe;
using namespace puntoexe::imebra;

#include <string>
#include "../Iterators/FragmentIterator.h"
#include "../Iterators/SequentialIterator.h"
#include "../Iterators/UpDownIterator.h"
#include "../Protocol/stream.pb.h"

// iterator for image frame
typedef SequentialIterator<Protocol::FrameFragment, FragmentIterator> FrameIterator;

//iterator for either a series of single frame images, or a single multi-frame image
typedef UpDownIterator<Protocol::FrameFragment, FrameIterator> ImageIterator;


class DicomPixels {
public:
	DicomPixels(string fileName);
	virtual ~DicomPixels();

	imbxUint32 getnumberOfFrames(){return numberOfFrames;}
	imbxUint32 getimageSizeX(){return imageSizeX;}
	imbxUint32 getimageSizeY(){return imageSizeY;}
	image::bitDepth getdepth(){return depth;}
	std::wstring getcolorSpace(){return colorSpace;}
	std::wstring gettransferSyntax(){return transferSyntax;}
	imbxUint8 getchannelsNumber(){return channelsNumber;}
	bool getbInterleaved(){return bInterleaved;}
	bool getb2Complement(){return b2Complement;}
	imbxUint8 getallocatedBits(){return allocatedBits;}
	imbxUint8 getstoredBits(){return storedBits;}
	imbxUint8 gethighBit(){return highBit;}
	bool getbSubSampledY(){return bSubSampledY;}
	bool getbSubSampledX(){return bSubSampledX;}

	//image iterator and file descriptor allow us to sendfile chunks of pixel data
	ImageIterator* getIterator(){ return iterator;}
	int getFileDescriptor(){ return fd;}

private:
	imbxUint32 numberOfFrames;
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


	int fd;;

	ImageIterator* iterator;

};

#endif /* DICOMPIXELS_H_ */
