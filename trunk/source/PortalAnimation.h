#pragma once

#include <Ogre.h>
#include <OgreNewt.h>
#include "ModelEntity.h"

class GameServices;
class InputManager;

class PortalAnimation : public ModelEntity {
public:
	PortalAnimation(GameServices *gs, Ogre::SceneNode *parentNode, const Ogre::Vector3 &pos, const Ogre::String &name, const Ogre::String &modelFile = "../assets/portal.mesh");
	~PortalAnimation();

	void update();

	Ogre::AnimationState* getAnimation();
	bool getStatus();

protected:
	Ogre::AnimationState *mAnimationState;

	float mTimer;
	bool complete;

};