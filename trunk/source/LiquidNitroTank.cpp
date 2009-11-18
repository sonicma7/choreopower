#include "LiquidNitroTank.h"
#include "GameServices.h"
#include "TriggerEvent.h"
#include "GameEvent.h"
#include "EventManager.h"

LiquidNitroTank::LiquidNitroTank(GameServices *gs, OgreNewt::World* collisionWorld, Ogre::SceneNode *parentNode, 
				const Ogre::Vector3 &pos, const Ogre::Vector3 &size, 
				const Ogre::String &entityName, const Ogre::String &modelFile, 
				const Ogre::String &listenfor, const Ogre::Vector3 &dest, float xspeed, float yspeed)
				: PuzzleEntity(gs, collisionWorld, parentNode, pos, size, entityName, modelFile
				,listenfor)
{
	mActive = false;
	mFalling = false;
	mX_Speed = 2.0;
	mY_Accel = 1.0;	//multiple of gravity
}
void LiquidNitroTank::update(){
	if (mActive) // move x
	{
		if (mFalling) // move y
		{
		}
	}
}
void LiquidNitroTank::listenCallback(GameEvent *evnt){

}
void LiquidNitroTank::registerListener(){}