#include "TriggerEntity.h"
#include "GameServices.h"
#include "TriggerEvent.h"
#include "GameEvent.h"
#include "EventManager.h"

TriggerEntity::TriggerEntity(GameServices *gs, OgreNewt::World* collisionWorld, Ogre::SceneNode *parentNode, 
							 const Ogre::Vector3 &pos, const Ogre::Vector3 &size, 
							 const Ogre::String &name, const Ogre::String &modelFile, 
							 const int triggerID)
: RigidModelEntity(gs, collisionWorld, parentNode, pos, size, name, modelFile),
	mTriggerID(triggerID) {
	mEntityType = ET_TriggetEntity; //some num that represents triggers
}

TriggerEntity::~TriggerEntity() {

}

void TriggerEntity::throwTrigger(){
	mGameServices->debugOut("real trigger");
	//can't get this to work with casting TriggerEvent, polymorphism?
	TriggerEvent* tevnt = new TriggerEvent("trigger",mTriggerID);
	mGameServices->events()->throwEvent((GameEvent*)tevnt);
}

void TriggerEntity::forceCallback(OgreNewt::Body *b) {
	throwTrigger();
}
void TriggerEntity::onCollisionEntered(RigidModelEntity *colEntity){	//should fire on entering collision
	RigidModelEntity::onCollisionEntered(colEntity);
	mGameServices->debugOut("collision entered with type:%i,%c",colEntity->EntityType(),colEntity->name());
	//mGameServices->events()->throwEvent(TriggerEvent("testEvent",1));
	throwTrigger();
}
void TriggerEntity::onCollisionExited(RigidModelEntity *colEntity){
	RigidModelEntity::onCollisionExited(colEntity);
	//mGameServices->debugOut("collision exited!");
}
