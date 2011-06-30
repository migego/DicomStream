/*
 * DicomStream.h
 *
 *  Created on: Jun 6, 2011
 *      Author: aboxer
 */

#ifndef DICOMSTREAM_H_
#define DICOMSTREAM_H_
#include <ev.h>
#include "eio.h"

#include "Iterators/concurrent_queue.h"
#include "Iterators/UpDownIterator.h"
#include "Iterators/SimpleIterator.h"
#include "Iterators/FragmentIterator.h"

#include "Protocol/MessageFramer.h"
#include "Dicom/DicomPixels.h"

#include "Iterators/SequentialIterator.h"
#include <map>
#include <vector>
using namespace std;

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

		}
		~TFileInfo()
		{
			if (parser)
				delete parser;
		}
		int fd;
		int refCount;
		DicomPixels* parser;

	};

	unsigned short port;

	struct TClient {
	        int fd;
	        ev_io ev_write;
			ev_io ev_read;
	};
	ev_io ev_accept;

	//singleton
	static DicomStream* instance;

	int acquire(string fileName);
	int release(string fileName);


	// message processing
	void createMessageFramer(int fd);
	void deleteMessageFramer(int fd);
	map<int, MessageFramer*> messageFramers;
	void processIncomingMessage(int clientFd, MessageFramer::GenericMessage msg);

	map<string, TFileInfo*> fileInfo;
	map<int, vector<FrameGroupIterator> > imageIterators; //key is client socket fd


	//precache
	concurrent_queue< string, UpDownIterator< string, SimpleIterator<string> > > precacheQueue;
	bool stopPrecache;
	string path;
	static void preFetch();
	void preFetch_();
	void clientTest_();
	static void clientTest();

	int setNonBlock(int fd);
	int setCork(int fd, bool cork);

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

    void want_poll_();
	void done_poll_();
    int open_cb_ (eio_req *req);
    int readahead_cb_ (eio_req *req);

	void unitTest();

};

#endif /* DICOMSTREAM_H_ */
