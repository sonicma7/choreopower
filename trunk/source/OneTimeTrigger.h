#pragma once
#include "PuzzleEntity.h"
#include <Ogre.h>
#include <OgreNewt.h>

class GameServices;
class TriggerEvent;
class EventManager;
//This switch only fires once the first time it is touched, then becoming inactice indefinitely
class OneTimeTrigger : public PuzzleEntity { //kinda need access to protected members...
public:
	OneTimeTrigger(GameServices *gs, OgreNewt::World* collisionWorld, Ogre::SceneNode *parentNode, 
					const Ogre::Vector3 &pos, const Ogre::Vector3 &size, 
					const Ogre::String &entityName, const Ogre::String &modelFile, 
					const Ogre::String &triggername, const int triggerID);
	void onCollisionEntered(RigidModelEntity *colEntity);
protected:
	bool mHasFired;
};