#pragma once

#include <Ogre.h>
#include <OgreNewt.h>
#include "RigidModelEntity.h"

class GameServices;
class InputManager;

class GroundAttackEntity : public RigidModelEntity {
public:
	GroundAttackEntity(GameServices *gs, OgreNewt::World *collisionWorld, Ogre::SceneNode *parentNode, const Ogre::Vector3 &pos, const Ogre::Vector3 &size, const Ogre::String &name, const Ogre::String &modelFile = "../assets/ground_attack.mesh");
	~GroundAttackEntity();

	void update();
	
	Ogre::AnimationState* getAnimation();
	bool getStatus();

	void onCollisionEntered(RigidModelEntity *colEntity);

protected:
	Ogre::AnimationState *mAnimationState;

	float mTimer;
	bool complete;

};