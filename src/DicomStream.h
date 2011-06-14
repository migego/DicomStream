/*
 * DicomStream.h
 *
 *  Created on: Jun 6, 2011
 *      Author: aboxer
 */

#ifndef DICOMSTREAM_H_
#define DICOMSTREAM_H_
#include <ev.h>

#include "Queue/concurrent_queue.h"
#include "Queue/UpDownIterator.h"
#include "Queue/SimpleFragmentIterator.h"

#include "Parse/MessageFramer.h"

#include <map>
using namespace std;

class DicomStream {
public:

	static DicomStream* Instance();
	void start();

private:

	DicomStream();
	virtual ~DicomStream();

	unsigned short port;
	string path;

	struct client {
	        int fd;
	        ev_io ev_write;
			ev_io ev_read;
	};

	static DicomStream* instance;

	ev_io ev_accept;

	void createMessageFramer(int fd);
	void deleteMessageFramer(int fd);
	map<int, MessageFramer*> messageFramers;
	void processIncomingMessage(MessageFramer::GenericMessage msg);



	concurrent_queue< string, UpDownIterator< string, SimpleFragmentIterator<string> > > precacheQueue;
	bool stopPrecache;

	int setnonblock(int fd);

	static void write_cb(struct ev_loop *loop, struct ev_io *w, int revents);
	static void read_cb(struct ev_loop *loop, struct ev_io *w, int revents);
	static void accept_cb(struct ev_loop *loop, struct ev_io *w, int revents);

	void write_cb_(struct ev_loop *loop, struct ev_io *w, int revents);
	void read_cb_(struct ev_loop *loop, struct ev_io *w, int revents);
	void accept_cb_(struct ev_loop *loop, struct ev_io *w, int revents);

	static void preFetch();
	static void clientTest();

	void preFetch_();
	void clientTest_();

	void unitTest();



};

#endif /* DICOMSTREAM_H_ */
