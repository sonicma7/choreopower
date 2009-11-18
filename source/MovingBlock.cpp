#include "MovingBlock.h"
#include "GameServices.h"
#include "TriggerEvent.h"
#include "GameEvent.h"
#include "EventManager.h"

MovingBlock::MovingBlock(GameServices *gs, OgreNewt::World* collisionWorld, Ogre::SceneNode *parentNode, 
					const Ogre::Vector3 &pos, const Ogre::Vector3 &size, 
					const Ogre::String &entityName, const Ogre::String &modelFile, 
					const Ogre::String &listenfor, const Ogre::Vector3 &dest)
					: PuzzleEntity(gs,collisionWorld,parentNode,pos,size,entityName,modelFile,listenfor) 
{//Only listens, does not send triggers
	mDest = dest;
	registerListener();
}

void MovingBlock::moveToDest(){
//moves self to destination position. Work on this later to get a smooth motion.
	//mGameServices->debugOut("MovingBlock relocating to destination");
	this->setPos(mDest);
	//this->update();
}
void MovingBlock::listenCallback(GameEvent *evnt){
//execute moveToDest() when recieve
	//mGameServices->debugOut("%s recieved trigger event: %s", this->mName.c_str(),mListenFor.c_str());
	moveToDest();
}
void MovingBlock::registerListener(){
	//mGameServices->debugOut("%s registered listener for: %s",this->mName.c_str(), mListenFor.c_str());
	mGameServices->events()->addListener(mListenFor, this, &MovingBlock::listenCallback);
}