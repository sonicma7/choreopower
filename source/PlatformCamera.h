#pragma once
#include <Ogre.h>
#include "GameServices.h"
#include "PlayerEntity.h"
// this class will act as an interface to the camera, so that we don't have to 
// move it around manually. All camera tracking and movement should be done in update().
class PlatformCamera {
public:
	PlatformCamera(Ogre::SceneNode *camNode, PlayerEntity *follow);
	void update();
protected:
	PlayerEntity* mFollow;
	Ogre::SceneNode* mCamNode;
};