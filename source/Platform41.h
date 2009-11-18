#pragma once
#include "PuzzleEntity.h"
#include <Ogre.h>
#include <OgreNewt.h>

class Platform41 : public PuzzleEntity{
public:
	Platform41(GameServices *gs, OgreNewt::World* collisionWorld, Ogre::SceneNode *parentNode, 
		const Ogre::Vector3 &pos, const Ogre::Vector3 &size, 
		const Ogre::String &entityName, const Ogre::String &modelFile, 
		const Ogre::String &listenfor);	//rest of parameters are going to be hardcoded in
	void update();
	void listenCallback(GameEvent *evnt);
	void forceCallback(OgreNewt::Body* b);
	void setOrientation(Ogre::Quaternion &orient);
private:
	Ogre::Quaternion mOrient;
	bool mInPhase1;
	Ogre::Vector3 mDest1;
	bool mInPhase2;
	Ogre::Vector3 mDest2;
	bool mInPhase3;
	Ogre::Vector3 mDest3;
	bool mActive;
	bool mLockX;
	bool mLockY;
	bool mLockZ;
	bool mPressed1A;
	bool mPressed1B;
	bool mPressed2;
	bool mPressed3;
	float mSpeed;
	std::string mListenFor2;
	std::string mListenFor3;
};