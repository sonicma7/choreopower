#include "Drawbridge.h"
#include "GameServices.h"
#include "TriggerEvent.h"
#include "GameEvent.h"
#include "EventManager.h"

Drawbridge::Drawbridge(GameServices *gs, OgreNewt::World* collisionWorld, Ogre::SceneNode *parentNode, 
							   const Ogre::Vector3 &pos, const Ogre::Vector3 &size, const Ogre::String &entityName, 
							   const Ogre::String &modelFile, const Ogre::String &listenfor)
							   :PuzzleEntity(gs, collisionWorld, parentNode, pos, size, entityName
							   , modelFile, listenfor)
{
	registerListener();
	mPosInit = pos;
	mOrientFlat = mDummyNode->getOrientation();
}
void Drawbridge::listenCallback(GameEvent *evnt){
	mDummyNode->setOrientation(mOrientFlat);
	setPos(Ogre::Vector3(mPosInit.x+10,mPosInit.y-25,mPosInit.z));
	updateNewtonBody();
}
void Drawbridge::setOrientation(Ogre::Quaternion &orient){
	mOrientUp = orient;
}