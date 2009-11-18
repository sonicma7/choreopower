// GameServices
// GameServices provides access to class ands functions that need
// to be accessed on a large scale by many other classes. An
// example of this is the input class.
// GameServices should be 'injected', or passed to every class in
// the game. This way every class has access to these objects and
// functions.

#pragma once

#include <Ogre.h>
#include "GameConfig.h"

class EventManager;
class InputManager;
class Game;

class GameServices {
public:
	GameServices(Ogre::Root *ogreRoot);
	~GameServices();

	void update();
	void setActive(bool b);
	void debugOut(const char* fmt, ...);
	void setGame(Game *game);
	InputManager* input() const;
	EventManager* events() const;
	Ogre::Root* ogreRoot() const;
	GameConfig* gameConfig() const;

private:
	bool mActive;
	InputManager *mInputManager;
	EventManager *mEventManager;
	Ogre::Root *mOgreRoot;
	Game *mGame;
	GameConfig *mGameConfig;
};