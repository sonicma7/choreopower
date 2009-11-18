#include "GameListener.h"
#include "Game.h"
#include "GameServices.h"
#include "InputManager.h"

GameListener::GameListener(Ogre::Root *ogreRoot) {
	mGameServices = new GameServices(ogreRoot);
	mGame = new Game(ogreRoot, mGameServices);
	//mGameServices->setGame(mGame);
}

GameListener::~GameListener() {
	delete mGame;
	delete mGameServices;
}

bool GameListener::frameStarted(const Ogre::FrameEvent &evnt) {
	mGameServices->update();
	mGame->startUpdate();

	return !mGameServices->input()->keyDown(OIS::KC_ESCAPE);
}

bool GameListener::frameEnded(const Ogre::FrameEvent &evnt) {
	mGame->endUpdate();

	return true;
}