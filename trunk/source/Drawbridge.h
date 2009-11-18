#pragma once
#include "PuzzleEntity.h"
#include <Ogre.h>
#include <OgreNewt.h>

class Drawbridge : public PuzzleEntity{
public:
	Drawbridge(GameServices *gs, OgreNewt::World* collisionWorld, Ogre::SceneNode *parentNode, 
		const Ogre::Vector3 &pos, const Ogre::Vector3 &size, 
		const Ogre::String &entityName, const Ogre::String &modelFile, 
		const Ogre::String &listenfor);
	void listenCallback(GameEvent *evnt);
	void setOrientation(Ogre::Quaternion &orient);
private:
	Ogre::Vector3 mPosInit;
	Ogre::Quaternion mOrientFlat;
	Ogre::Quaternion mOrientUp;
};