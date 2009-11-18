// GameListener
// The game listener is the connection between the ogre render loop
// and the game. The game listener is notified at the start and
// end of every frame. The game can then do w/e ever it needs to
// do between the beginning and end of frame.

#ifndef GAMELISTENER_H
#define GAMELISTENER_H

#include <OgreFrameListener.h>

class Game;
class GameServices;

class GameListener : public Ogre::FrameListener {
public:
	GameListener(Ogre::Root *ogreRoot);
	~GameListener();

	bool frameStarted(const Ogre::FrameEvent &evnt);
	bool frameEnded(const Ogre::FrameEvent &evnt);

private:
	Game *mGame;
	GameServices *mGameServices;
};

#endif