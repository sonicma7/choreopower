#pragma once
#include "PuzzleEntity.h"
#include <Ogre.h>
#include <OgreNewt.h>

class GameServices;
class TriggerEvent;
class EventManager;

//this object keeps sending out trigger events as long as it is in collision 
class PressureSensor : public PuzzleEntity {
public:
	PressureSensor(GameServices *gs, OgreNewt::World* collisionWorld, Ogre::SceneNode *parentNode, 
					const Ogre::Vector3 &pos, const Ogre::Vector3 &size, 
					const Ogre::String &entityName, const Ogre::String &modelFile, 
					const Ogre::String &triggername, const int triggerID);
	virtual void onCollisionEntered(RigidModelEntity *colEntity);
};