#include "PressureSensor.h"
#include "GameServices.h"
#include "TriggerEvent.h"
#include "GameEvent.h"
#include "EventManager.h"

PressureSensor::PressureSensor(GameServices *gs, OgreNewt::World *collisionWorld, Ogre::SceneNode *parentNode, 
							   const Ogre::Vector3 &pos, const Ogre::Vector3 &size, 
							   const Ogre::String &entityName, const Ogre::String &modelFile, 
							   const Ogre::String &triggername, const int triggerID)
	:PuzzleEntity(gs,collisionWorld,parentNode, pos, size, entityName, modelFile, triggername,triggerID)
{

}
void PressureSensor::onCollisionEntered(RigidModelEntity *colEntity){
	RigidModelEntity::onCollisionEntered(colEntity);
	if(colEntity->getType() == ET_PlayerEntity){
		//mGameServices->debugOut("throwing trigger!");
		throwTrigger();
	}
}