#pragma once
#include "PuzzleEntity.h"
#include <Ogre.h>
#include <OgreNewt.h>
// platform starts rising when trigggered but sinks otherwise

class RisingPlatform : public PuzzleEntity{
public:
	RisingPlatform(GameServices *gs, OgreNewt::World* collisionWorld, Ogre::SceneNode *parentNode, 
		const Ogre::Vector3 &pos, const Ogre::Vector3 &size, 
		const Ogre::String &entityName, const Ogre::String &modelFile, 
		const Ogre::String &listenfor);
	void update();
	void listenCallback(GameEvent *evnt);
	void forceCallback(OgreNewt::Body* b);
	void setOrientation(Ogre::Quaternion &orient);
private:
	Ogre::Quaternion mOrient;
	bool mRising;
	bool mFalling;
	float mRiseSpeed;
	float mFallSpeed;
	Ogre::Vector3 mInitPos;
	Ogre::Vector3 mFinalPos;
	bool mButtonPressed;
};