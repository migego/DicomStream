/*
 * protocol.h
 *
 *  Created on: May 22, 2011
 *      Author: aboxer
 */

#ifndef PROTOCOL_H_
#define PROTOCOL_H_

using namespace std;
#include <string>
#include <list>


/*


TAG(byte) SIZE(unsigned short) VALUE(bytes)


 */

///////////////////////////////////////////////////
// Request Messages////////////////////////////////

enum Priority
{
	Selected,
	Visible,
	Hidden
};


enum Request
{
	//////////////////
	// request series
	SeriesRequest,
	SeriesUid,
	SeriesNumber,
	InstanceUid,
	InstanceNumber,
	NumberOfFrames,
	SeriesPriority, //see priority above

	/////////////////////
	// change priority
	SeriesPriorityChanged,


	//////////////////////////////////////
	//set series primary frame (default is 0)
	SetPrimaryFrame, // instance and frame number

    /////////////////////////////
	// cancel (cancel series or study)
	Cancel


};

struct FrameRequest
{
	string seriesUid;
	long seriesNumber;  // used to avoid passing series uid every time
	string instanceUid;
	long instanceNumber;  // used to avoid passing instance uid every time
	list<int> frameNumbers;
	Priority priority;
};

struct Frames
{
	string studyUid;
	string seriesUid;
	list<FrameRequest> frames;

};
///////////////////////////////////////////////////////

// Response Messages
enum Response
{
	///////////////
	FrameHeader,
	PixelDataSize,
	Width,
	Height,
	// ......

	//////////////////
	FrameFragment,
	Offset,
	Size,


	///////////////////
	Error,
	NasTimeout,
	InstanceMissing,

	///////////////////
	OK


};

struct FrameHdr
{
	long pixelDataSize;
	int width;
	int height;
	//.... various basic dicom tags


};

struct FrameFrag
{
    long offset;
    long size;

};

struct FrameResponse : FrameFrag
{
	long seriesNumber;    // used to avoid passing series uid every time
	long instanceNumber;  // used to avoid passing instance uid every time
	long frameNumber;     // for multi-frame
};


#endif /* PROTOCOL_H_ */
