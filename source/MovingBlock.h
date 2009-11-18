//block moves when recieves the registered trigger message, moves only once
#pragma once
#include "PuzzleEntity.h"
#include <Ogre.h>
#include <OgreNewt.h>

class GameServices;
class TriggerEvent;
class EventManager;

class MovingBlock : public PuzzleEntity {
public:
	MovingBlock(GameServices *gs, OgreNewt::World* collisionWorld, Ogre::SceneNode *parentNode, 
					const Ogre::Vector3 &pos, const Ogre::Vector3 &size, 
					const Ogre::String &entityName, const Ogre::String &modelFile, 
					const Ogre::String &listenfor, const Ogre::Vector3 &dest);
	//void update();
	void moveToDest();
	void listenCallback(GameEvent *evnt);	//executed when a listened for event arrives
	void registerListener();
protected:
	Ogre::Vector3 mDest;
};