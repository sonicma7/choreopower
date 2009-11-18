#pragma once
#include "PressureSensor.h"
#include <Ogre.h>
#include <OgreNewt.h>

class GameServices;
class TriggerEvent;
class EventManager;
// This object is for spike pits/water, anything that can kill a player on contact
// will send out a trigger message with player name appended to it.
class HazardTrigger : public PressureSensor {
public:
	HazardTrigger(GameServices *gs, OgreNewt::World* collisionWorld, Ogre::SceneNode *parentNode, 
		const Ogre::Vector3 &pos, const Ogre::Vector3 &size, 
		const Ogre::String &entityName, const Ogre::String &modelFile, 
		const Ogre::String &triggername);
	void onCollisionEntered(RigidModelEntity *colEntity);
	void throwTrigger(std::string pID);
};