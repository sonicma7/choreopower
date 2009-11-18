#pragma once

#include <Ogre.h>
#include <OgreNewt.h>
#include "RigidModelEntity.h"

class GameServices;
class InputManager;

class WindAttackEntity : public RigidModelEntity {
public:
	WindAttackEntity(GameServices *gs, OgreNewt::World *collisionWorld, Ogre::SceneNode *parentNode, const Ogre::Vector3 &pos, const Ogre::Vector3 &size, const Ogre::String &name, const Ogre::Radian direction, const Ogre::String &modelFile = "../assets/crescent.mesh");
	~WindAttackEntity();

	virtual void forceCallback(OgreNewt::Body* b);

	virtual void transformCallback(OgreNewt::Body *b, const Ogre::Quaternion&, const Ogre::Vector3&);

	void onCollisionEntered(RigidModelEntity *colEntity);

	bool getStatus();
	void update();

protected:
	Ogre::Timer *mTimer;
	float mWindSlash;
	bool complete;
	Ogre::Radian mDirection;

};