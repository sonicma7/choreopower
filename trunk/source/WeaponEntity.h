#pragma once

#include <Ogre.h>
#include <OgreNewt.h>
#include "ModelEntity.h"

class GameServices;
class InputManager;

class WeaponEntity : public ModelEntity {
public:
	WeaponEntity(GameServices *gs, Ogre::SceneNode *parentNode, const Ogre::Vector3 &pos, const Ogre::String &name, const Ogre::String &modelFile);
	~WeaponEntity();

	virtual void reattachNodeToBone(Ogre::Entity *ent, const Ogre::String &boneName);
};