#include "PlatformCamera.h"
#include "GameServices.h"
#include "PlayerEntity.h"

PlatformCamera::PlatformCamera(Ogre::SceneNode *camNode, PlayerEntity *follow)
{
	mFollow = follow;
	mCamNode = camNode;
}
void PlatformCamera::update()
{
	mCamNode->setPosition(mFollow->position().x,mFollow->position().y,mCamNode->getPosition().z);
}