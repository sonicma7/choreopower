#pragma once
#include "TriggerEvent.h"
#include "GameEvent.h"

TriggerEvent::TriggerEvent(const std::string &name, int ID)
: GameEvent(name), mID(ID) {

}