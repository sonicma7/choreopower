#include "EventManager.h"
#include <stack>

typedef std::multimap<std::string, EventFunctor*>::iterator EventMapItr;
typedef std::pair<EventMapItr, EventMapItr> EventMapItrPair;

EventManager::EventManager() {
	mEventsMap = new std::multimap<std::string, EventFunctor*>();
}

EventManager::~EventManager() {

}

/*void EventManager::addListener(std::string eventName, EventFunctor* funcPtr) {
	std::pair<std::string, EventFunctor*> mapPair(eventName, funcPtr);

	mEventsMap->insert( mapPair );
}*/

void EventManager::removeListener(std::string eventName, void* objectPtr) {
	EventMapItrPair rangePair = mEventsMap->equal_range(eventName);
	
	std::stack<EventMapItr> objectsToRemove;

	for(EventMapItr itr = rangePair.first; itr != rangePair.second; ++itr) {
		if(itr->second->objectPtr() == objectPtr) {
			objectsToRemove.push(itr);
		}
	}

	while(!objectsToRemove.empty()) {
		EventFunctor* functor = objectsToRemove.top()->second;
		delete functor;

		mEventsMap->erase(objectsToRemove.top());
		objectsToRemove.pop();
	}
}

void EventManager::throwEvent(GameEvent* evnt) {
	EventMapItrPair rangePair = mEventsMap->equal_range(evnt->name());
	
	std::stack<EventMapItr> callStack;

	for(EventMapItr itr = rangePair.first; itr != rangePair.second; ++itr) {
		callStack.push(itr);
	}

	while(!callStack.empty()) {
		callStack.top()->second->call(evnt);
		callStack.pop();
	}

	delete evnt;
}