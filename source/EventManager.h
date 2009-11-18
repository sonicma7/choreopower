#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

#include <string>
#include <map>

#include "EventFunctor.h"
#include "GameEvent.h"

class EventManager {
public:
	EventManager();
	~EventManager();
	
	template <class T>
	void addListener(std::string eventName, T *objectPtr, void (T::*funcPtr)(GameEvent *evnt)) {
		TEventFunctor<T> *functor = new TEventFunctor<T>(objectPtr, funcPtr);
		std::pair<std::string, EventFunctor*> mapPair(eventName, functor);

		mEventsMap->insert( mapPair );
	}

	void removeListener(std::string eventName, void* objectPtr);

	void throwEvent(GameEvent* evnt);

private:
	//key - message name (string)
	//value - object pointer
	std::multimap<std::string, EventFunctor*> *mEventsMap;
};

#endif