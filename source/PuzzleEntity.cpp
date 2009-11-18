#include "PuzzleEntity.h"
#include "GameServices.h"
#include "TriggerEvent.h"
#include "GameEvent.h"
#include "EventManager.h"


PuzzleEntity::PuzzleEntity(GameServices *gs, OgreNewt::World* collisionWorld, Ogre::SceneNode *parentNode, 
					const Ogre::Vector3 &pos, const Ogre::Vector3 &size, 
					const Ogre::String &entityName, const Ogre::String &modelFile, 
					const Ogre::String &triggername, const Ogre::String &listenfor, const int triggerID)
	: TriggerEntity(gs, collisionWorld, parentNode, 
					pos, size, 
					entityName, modelFile, triggerID) {
	//this one throws and listens for trigger events
	mThrowsTriggers = true;
	mRegistersListener = true;
	mTriggerName = triggername;
	mListenFor = listenfor;
	mEntityType = ET_PuzzleEntity;
}
PuzzleEntity::PuzzleEntity(GameServices *gs, OgreNewt::World* collisionWorld, Ogre::SceneNode *parentNode, 
					const Ogre::Vector3 &pos, const Ogre::Vector3 &size, 
					const Ogre::String &entityName, const Ogre::String &modelFile, 
					const Ogre::String &triggername, const int triggerID)
	: TriggerEntity(gs, collisionWorld, parentNode, 
					pos, size, 
					entityName, modelFile, triggerID) {
	//throws but does not listen for trigger events
	mThrowsTriggers = true;
	mRegistersListener = false;
	mTriggerName = triggername;
	mListenFor = "none";
	mEntityType = ET_PuzzleEntity;
}
PuzzleEntity::PuzzleEntity(GameServices *gs, OgreNewt::World* collisionWorld, Ogre::SceneNode *parentNode, 
					const Ogre::Vector3 &pos, const Ogre::Vector3 &size, 
					const Ogre::String &entityName, const Ogre::String &modelFile, 
					const Ogre::String &listenfor)
	: TriggerEntity(gs, collisionWorld, parentNode, 
					pos, size, 
					entityName, modelFile, 0) {
	//only listens for trigger events, does not throw any, set triggerID=0, shouldn't see any thrown
	mThrowsTriggers = false;
	mRegistersListener = true;
	mTriggerName = "none";
	mListenFor = listenfor;
	mEntityType = ET_PuzzleEntity;
}
PuzzleEntity::~PuzzleEntity(){
	if(mRegistersListener)
		mGameServices->events()->removeListener(mListenFor, this);
}
const std::string PuzzleEntity::getType() const {
	return "Puzzle";
}
void PuzzleEntity::throwTrigger(){
	//mGameServices->debugOut("throwing trigger: %s", mTriggerName.c_str());
	TriggerEvent* tevnt = new TriggerEvent(mTriggerName,mTriggerID);
	mGameServices->events()->throwEvent((GameEvent*)tevnt);
}
void PuzzleEntity::listenCallback(GameEvent *evnt){
	//mGameServices->debugOut("PuzzleEntity recieved trigger event: %s", mListenFor.c_str());
}
void PuzzleEntity::registerListener(){
	//mGameServices->debugOut("%s in PuzzleEntity registered listener for: %s",this->mName.c_str(), mListenFor.c_str());
	mGameServices->events()->addListener(mListenFor, this, &PuzzleEntity::listenCallback);
}
void PuzzleEntity::onCollisionEntered(RigidModelEntity *colEntity){
	RigidModelEntity::onCollisionEntered(colEntity);
}

void PuzzleEntity::onCollisionExited(RigidModelEntity *colEntity){
	RigidModelEntity::onCollisionExited(colEntity);
}