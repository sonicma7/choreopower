// Game
// This is the main starting point for all game related anything.
// All major game systems should be initialized and updated in
// this class. This class is only for in game related systems, and
// should only be responsible for larger systems and managers.

#pragma once

#include <Ogre.h>

class Level;
class GameServices;
class TextRenderer;

class Game {
public:
	Game(Ogre::Root *ogreRoot, GameServices *gameServices);
	~Game();
	
	void init();
	void startUpdate();
	void endUpdate();

private:


	TextRenderer *mTextRenderer;
	Ogre::Root *mOgreRoot;
	GameServices *mGameServices;
	Ogre::RenderWindow* mWindow;
	Level *mLevel;
	unsigned int p1_HP;
	unsigned int p2_HP;
	Ogre::Timer *mGameClock;
	Ogre::Timer *mFrameTimer;
	float mTargetFPS;
};
