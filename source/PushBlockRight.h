#pragma once
#include "PuzzleEntity.h"
#include <Ogre.h>
#include <OgreNewt.h>
class PressureSensor;
// This object is a block that can be pushed to the right by players, until it reaches destination
/* Implementation: block will be rigid body, have an invisible PressureSensor to the left of the
*	block. Both the RigidBody and the PressureSensor move to the right on player contact with 
*	the PressureSensor. */
class PushBlockRight : public PuzzleEntity{
public:
	PushBlockRight(GameServices *gs, OgreNewt::World* collisionWorld, Ogre::SceneNode *parentNode, 
		const Ogre::Vector3 &pos, const Ogre::Vector3 &size, 
		const Ogre::String &entityName, const Ogre::String &modelFile, 
		const Ogre::String &triggername, const Ogre::String &listenfor, const int triggerID,
		const Ogre::Vector3 &dest, float pushSpeed);
	void update();
	void listenCallback(GameEvent *evnt);
	void forceCallback(OgreNewt::Body* b);
private:
	float mSpeed;
	bool mBump;
	Ogre::Vector3 mSize;
	Ogre::Vector3 mDest;
	PressureSensor *mSensor;
};