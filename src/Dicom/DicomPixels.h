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

#include "ParseListenManager.h"



class DicomPixels {
public:
	DicomPixels(){}
	virtual ~DicomPixels();

	void parse(int imageFileDescriptor, string fileName, ParseListenManager* listenManager);

	imbxUint32 getnumberOfFrames(){return numberOfFrames;}
	imbxUint32 getimageSizeX(){return imageSizeX;}
	imbxUint32 getimageSizeY(){return imageSizeY;}
	image::bitDepth getdepth(){return depth;}
	std::string getcolorSpace(){return colorSpace;}
	std::string gettransferSyntax(){return transferSyntax;}
	imbxUint8 getchannelsNumber(){return channelsNumber;}
	bool getbInterleaved(){return bInterleaved;}
	bool getb2Complement(){return b2Complement;}
	imbxUint8 getallocatedBits(){return allocatedBits;}
	imbxUint8 getstoredBits(){return storedBits;}
	imbxUint8 gethighBit(){return highBit;}

private:
	imbxUint32 numberOfFrames;
	imbxUint32 imageSizeX;
	imbxUint32 imageSizeY;
	image::bitDepth depth;
	std::string colorSpace;
	std::string transferSyntax;
	imbxUint8 channelsNumber;
	bool bInterleaved;
	bool b2Complement;
	imbxUint8 allocatedBits;
	imbxUint8 storedBits;
	imbxUint8 highBit;


	vector< TParsedFrame* > frameFragments;

};

#endif /* DICOMPIXELS_H_ */
