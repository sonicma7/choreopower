#ifndef GAMEEVENT_H
#define GAMEEVENT_H

#include <string>

class GameEvent {
public:
	GameEvent(const std::string &name);

	const std::string& name() const;

protected:
	std::string mName;
};

#endif