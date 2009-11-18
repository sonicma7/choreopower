#include "HazardTrigger.h"
#include "GameServices.h"
#include "TriggerEvent.h"
#include "GameEvent.h"
#include "EventManager.h"

HazardTrigger::HazardTrigger(GameServices *gs, OgreNewt::World* collisionWorld, Ogre::SceneNode *parentNode, 
			  const Ogre::Vector3 &pos, const Ogre::Vector3 &size, 
			  const Ogre::String &entityName, const Ogre::String &modelFile, 
			  const Ogre::String &triggername)
			  : PressureSensor(gs, collisionWorld, parentNode, pos, size, entityName, modelFile, triggername, 4) //triggerid shouldn't matter here
{
	
}
void HazardTrigger::onCollisionEntered(RigidModelEntity *colEntity){
	RigidModelEntity::onCollisionEntered(colEntity);
	if(colEntity->getType() == ET_PlayerEntity){
		throwTrigger(colEntity->name());
	}
}
void HazardTrigger::throwTrigger(std::string pID)
{
	//std::string triggerMessage = mTriggerName + pID;
	//mGameServices->debugOut("Hazard: %s", triggerMessage.c_str());
	//TriggerEvent* tevnt = new TriggerEvent(mTriggerName + pID,mTriggerID);
	//mGameServices->events()->throwEvent((GameEvent*)tevnt);
}