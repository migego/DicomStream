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

	struct TClientInfo
	{
		TClientInfo() : messageFramer(NULL), pendingSeriesInstanceUid("")
		{

		}
		vector<FrameGroupIterator*> frameGroupQueue;
		Protocol::FrameFragmentHeader currentFragment;
		MessageFramer* messageFramer;
		string pendingSeriesInstanceUid;
	};

	struct TFadvise
	{
		TFadvise(int fd, off_t offset, off_t len, int advice) : fd(fd), offset(offset), len(len), advice(advice)
		{

		}
 		int fd;
		off_t offset;
		off_t len;
		int advice;
	};

	struct TFileInfo

	{
		TFileInfo() : fileName(""), fd(-1), refCount(0), parser(NULL) {}
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
		    TClient() : fd(-1), frameGroup(NULL) {}
		    TClient(TClient* that)
		    {
		    	fd = that->fd;
		    	frameGroup = that->frameGroup;
		    }
	        int fd;
			FrameGroupIterator* frameGroup;
	        ev_io ev_write;
			ev_io ev_read;
	};

	void cleanup(TClient* cli);


	struct TEio
	{
		TEio() : cli(NULL), offset(0), size(0) {}
		TClient* cli;
		unsigned int offset;
		unsigned int size;
	};

	ev_io ev_accept;

	//singleton
	static DicomStream* instance;

	//IFileRefCounter implementation
	int acquire(string fileName);
	int release(string fileName);
	int refCount(string fileName);

    TFileInfo* getFileInfo(string fileName);
    TFileInfo* getFileInfo(FrameGroupIterator* frameGroup);
	// message processing
	void createMessageFramer(int fd);
	void deleteMessageFramer(int fd);
	MessageFramer* getMessageFramer(int fd);
	void processIncomingMessage(DicomStream::TClient* cli, MessageFramer::MessageWrapper msg);

	void processPendingSetPriorityRequest(int clientFd);
	// parsing
	map<int, TClientInfo*  > clientInfoMap; //key is client fd
	map<string, TFileInfo*> fileInfo;  // key is file name
	ParseListenManager listenManager;
    void triggerOpenOrWrite(int fd);

	string path;

	/////////// Client Test //////////////////
	int clientFd;
	void clientTest_();
	static void clientTest();
	void clientTestRead_();
	static void clientTestRead();
	/////////////////////////////////////////////////


	int setNonBlock(int fd);
	int setCork(int clientFd, bool cork);

	static void write_cb(struct ev_loop *loop, struct ev_io *w, int revents);
	static void read_cb(struct ev_loop *loop, struct ev_io *w, int revents);
	static void accept_cb(struct ev_loop *loop, struct ev_io *w, int revents);

	void write_cb_(struct ev_loop *loop, struct ev_io *w, int revents);
	void read_cb_(struct ev_loop *loop, struct ev_io *w, int revents);
	void accept_cb_(struct ev_loop *loop, struct ev_io *w, int revents);

	void triggerWrite(struct ev_loop *loop, TClient* cli);
	void triggerRead(struct ev_loop *loop, TClient* cli);

	static void want_poll();
	static void done_poll();
	static int open_cb (eio_req *req);
	static int readahead_cb (eio_req *req);
	static int sendfile_cb (eio_req *req);
	static void eio_fadvise(eio_req *req);
	static int eio_fadvise_cb(eio_req *req);

    void want_poll_();
	void done_poll_();
    int open_cb_ (eio_req *req);
    int readahead_cb_ (eio_req *req);
    int sendfile_cb_ (eio_req *req);
    void eio_fadvise_(eio_req *req);
    int eio_fadvise_cb_(eio_req *req);

	void unitTest();

};

#endif /* DICOMSTREAM_H_ */
