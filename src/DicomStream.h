/*
 * DicomStream.h
 *
 *  Created on: Jun 6, 2011
 *      Author: aboxer
 */

#ifndef DICOMSTREAM_H_
#define DICOMSTREAM_H_

#include <map>
#include <vector>
#include <queue>
using namespace std;


#include <ev.h>
#include "eio.h"

#include "Iterators/UpDownIterator.h"
#include "Iterators/SimpleIterator.h"
#include "Iterators/FragmentIterator.h"
#include "Iterators/FrameGroupIterator.h"
#include "Iterators/SequentialIterator.h"

#include "Protocol/MessageFramer.h"
#include "Dicom/DicomPixels.h"
#include "Dicom/ParseListenManager.h"

#include "IFileRefCounter.h"


class DicomStream : public IFileRefCounter {
public:

	static DicomStream* Instance();
	void start();

private:

	DicomStream();
	virtual ~DicomStream();

	struct TFileInfo
	{
		TFileInfo()
		{
			fd = -1;
			parser = NULL;
			fileName="";
		}
		~TFileInfo()
		{
			if (parser)
				delete parser;
		}
		string fileName;
		int fd;
		int refCount;
		DicomPixels* parser;

	};
	void cleanup(string fileName);


	unsigned short port;

	struct TClient {
	        int fd;
	        ev_io ev_write;
			ev_io ev_read;
	};

	void cleanup(TClient* cli);


	struct TEio
	{
		TClient* cli;
		TFileInfo* fileInfo;
	};

	ev_io ev_accept;

	//singleton
	static DicomStream* instance;

	//IFileRefCounter implementation
	int acquire(string fileName);
	int release(string fileName);
	int refCount(string fileName);

    TFileInfo* getFileInfo(string fileName);

	// message processing
	void createMessageFramer(int fd);
	void deleteMessageFramer(int fd);
	map<int, MessageFramer*> messageFramers;
	void processIncomingMessage(DicomStream::TClient* cli, MessageFramer::MessageWrapper msg);

	// parsing
	map<int, queue<FrameGroupIterator*>*  > frameGroupIterators; //key is client fd
	map<string, TFileInfo*> fileInfo;  // key is file name
	ParseListenManager listenManager;
    void triggerNextEvent(TClient* cli, FrameIterator* frameIter);

	string path;

	int clientFd;
	void clientTest_();
	static void clientTest();
	void clientTestRead_();
	static void clientTestRead();


	int setNonBlock(int fd);
	int setCork(int clientFd, bool cork);

	static void write_cb(struct ev_loop *loop, struct ev_io *w, int revents);
	static void read_cb(struct ev_loop *loop, struct ev_io *w, int revents);
	static void accept_cb(struct ev_loop *loop, struct ev_io *w, int revents);

	void write_cb_(struct ev_loop *loop, struct ev_io *w, int revents);
	void read_cb_(struct ev_loop *loop, struct ev_io *w, int revents);
	void accept_cb_(struct ev_loop *loop, struct ev_io *w, int revents);

	static void want_poll();
	static void done_poll();
	static int open_cb (eio_req *req);
	static int readahead_cb (eio_req *req);
	static int sendfile_cb (eio_req *req);

    void want_poll_();
	void done_poll_();
    int open_cb_ (eio_req *req);
    int readahead_cb_ (eio_req *req);
    int sendfile_cb_ (eio_req *req);

	void unitTest();

};

#endif /* DICOMSTREAM_H_ */
