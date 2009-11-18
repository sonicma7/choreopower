#include "GameEvent.h"

GameEvent::GameEvent(const std::string &name) {
	mName = name;
}

const std::string& GameEvent::name() const {
	return mName;
}