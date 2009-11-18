#include "TwoHitMover.h"
#include "GameServices.h"
#include "TriggerEvent.h"
#include "GameEvent.h"
#include "EventManager.h"

TwoHitMover::TwoHitMover(GameServices *gs, OgreNewt::World* collisionWorld, Ogre::SceneNode *parentNode, 
						 const Ogre::Vector3 &pos, const Ogre::Vector3 &size, 
						 const Ogre::String &entityName, const Ogre::String &modelFile, 
						 const Ogre::String &listenfor, const Ogre::Vector3 &dest)
						 : PuzzleEntity(gs,collisionWorld,parentNode,pos,size,entityName,modelFile,listenfor) 
{//Only listens, does not send triggers
	mDest = dest;
	hits = 0;
	registerListener();
}

void TwoHitMover::moveToDest(){
	//moves self to destination position. Work on this later to get a smooth motion.
	//mGameServices->debugOut("TwoHitMover relocating to destination");
	this->setPos(mDest);
	//this->update();
}
void TwoHitMover::listenCallback(GameEvent *evnt){
	//execute moveToDest() when recieve
	//mGameServices->debugOut("%s recieved trigger event: %s", this->mName.c_str(),mListenFor.c_str());
	hits++;
	if (hits > 1){
		moveToDest();
	}
}
void TwoHitMover::registerListener(){
	//mGameServices->debugOut("%s registered listener for: %s",this->mName.c_str(), mListenFor.c_str());
	mGameServices->events()->addListener(mListenFor, this, &TwoHitMover::listenCallback);
}