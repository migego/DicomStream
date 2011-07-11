/*
 * ParseListenManager.h
 *
 *  Created on: Jun 29, 2011
 *      Author: aboxer
 */

#ifndef PARSELISTENMANAGER_H_
#define PARSELISTENMANAGER_H_

#include "IParseListener.h"
#include <set>

class ParseListenManager {
public:
	ParseListenManager(){}
	virtual ~ParseListenManager(){

		map<string, set<IParseListener*>* >::iterator iter = parseListeners.begin();
		while (iter != parseListeners.end())
		{
			delete iter->second;
		}
	}

	bool addListener(string fileName, IParseListener* listener)
	{
		set<IParseListener*>* listeners = NULL;
		if (parseListeners.find(fileName) == parseListeners.end())
		{
           listeners = new set<IParseListener*>();
           parseListeners[fileName] = listeners;
		}
		else
		{
			listeners = parseListeners[fileName];
		}
        return listeners->insert(listener).second;
	}
	size_t removeListener(string fileName, IParseListener* listener)
	{
		if (parseListeners.find(fileName) == parseListeners.end())
		{
		   return 0;
		}

		set<IParseListener*>*  listeners = parseListeners[fileName];
		return listeners->erase(listener);

	}
	void notifyListeners(string fileName, vector< TParsedFrame* >& frameFragments)
	{
		if (parseListeners.find(fileName) == parseListeners.end())
		{
		   return;
		}
		set<IParseListener*>*  listeners = parseListeners[fileName];
		set<IParseListener*>::iterator iter = listeners->begin();
		while (iter != listeners->end())
		{
			(*iter)->parsed(frameFragments);
			iter++;
		}
	}
private:
	map<string, set<IParseListener*>* > parseListeners;

};

#endif /* PARSELISTENMANAGER_H_ */
