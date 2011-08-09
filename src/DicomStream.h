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
#include "DataStructs.h"


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

	enum ePendingOpenClose
	{
		FILE_NONE,
		FILE_OPEN,
		FILE_CLOSE
	};

	struct TFileInfo

	{
		TFileInfo() : fileName(""), fd(-1), refCount(0),
				      pending(FILE_NONE),
				      nextPending(FILE_NONE),
				      parser(NULL)
		{}
		~TFileInfo()
		{
			if (parser)
				delete parser;
		}
		string fileName;
		int fd;
		int refCount;
		ePendingOpenClose pending;
		ePendingOpenClose nextPending;
		DicomPixels* parser;

	};
	void cleanup(string fileName);


	unsigned short port;

	struct TClient : public TClientData {
		TClient(int sockFd) : TClientData(sockFd, NULL) {}
		TClient(int sockFd, FrameGroupIterator* fGroup) : TClientData(sockFd, fGroup) {}
		TClient(TClientData* data) : TClientData(data) {}
		ev_io ev_write;
		ev_io ev_read;
	};

	void cleanup(TClient* cli);

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
	int testClientSocketFd;
	void clientTest_();
	static void clientTest();
	void clientTestRead_();
	static void clientTestRead();

	struct TClientResponseInfo
	{
		TClientResponseInfo() : frameResponse(NULL), frameFragment(NULL), totalCount(0){}

		Protocol::FrameResponse* frameResponse;
		Protocol::FrameFragmentHeader* frameFragment;
		int totalCount;
	};
	map<string, TClientResponseInfo*> clientResponseInfoMap;
	/////////////////////////////////////////////////

    ev_async my_eio_sig;

	static void my_eio_sig_cb (EV_P_ ev_async *w, int revents);
    void my_eio_sig_cb_ (EV_P_ ev_async *w, int revents);

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
	static int close_cb (eio_req *req);
	static int readahead_cb (eio_req *req);
	static int sendfile_cb (eio_req *req);
	static void eio_fadvise(eio_req *req);
	static int eio_fadvise_cb(eio_req *req);

    void want_poll_();
	void done_poll_();
    int open_cb_ (eio_req *req);
    int close_cb_ (eio_req *req);
    int readahead_cb_ (eio_req *req);
    int sendfile_cb_ (eio_req *req);
    void eio_fadvise_(eio_req *req);
    int eio_fadvise_cb_(eio_req *req);

	void unitTest();

	void open(TClient* cli, string fileName);

};

#endif /* DICOMSTREAM_H_ */
