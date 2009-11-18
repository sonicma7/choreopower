#include "OneTimeTrigger.h"
#include "GameServices.h"
#include "TriggerEvent.h"
#include "GameEvent.h"
#include "EventManager.h"

OneTimeTrigger::OneTimeTrigger(GameServices *gs, OgreNewt::World *collisionWorld, Ogre::SceneNode *parentNode, 
							   const Ogre::Vector3 &pos, const Ogre::Vector3 &size, 
							   const Ogre::String &entityName, const Ogre::String &modelFile, 
							   const Ogre::String &triggername, const int triggerID)
	:PuzzleEntity(gs,collisionWorld,parentNode, pos, size, entityName, modelFile, triggername,triggerID)
{
	mHasFired = false;
}
void OneTimeTrigger::onCollisionEntered(RigidModelEntity *colEntity){
	RigidModelEntity::onCollisionEntered(colEntity);
	if(!mHasFired){
		if((colEntity->getType() == ET_PlayerEntity)||(colEntity->getType() == ET_FireEntity)){
			throwTrigger();
			mHasFired = true;
		}
	}
}