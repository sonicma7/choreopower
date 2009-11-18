#pragma once
#include <string>
#include "GameEvent.h"

class TriggerEvent : public GameEvent {
public:
	TriggerEvent(const std::string &name, int ID);
	const int mID;
};