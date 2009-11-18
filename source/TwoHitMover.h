//block moves when recieves the registered trigger message TWICE, moves only once
#pragma once
#include "PuzzleEntity.h"
#include <Ogre.h>
#include <OgreNewt.h>

class GameServices;
class TriggerEvent;
class EventManager;

class TwoHitMover : public PuzzleEntity {
public:
	TwoHitMover(GameServices *gs, OgreNewt::World* collisionWorld, Ogre::SceneNode *parentNode, 
		const Ogre::Vector3 &pos, const Ogre::Vector3 &size, 
		const Ogre::String &entityName, const Ogre::String &modelFile, 
		const Ogre::String &listenfor, const Ogre::Vector3 &dest);
	void moveToDest();
	void listenCallback(GameEvent *evnt);	//executed when a listened for event arrives
	void registerListener();
protected:
	Ogre::Vector3 mDest;
	unsigned int hits;
};