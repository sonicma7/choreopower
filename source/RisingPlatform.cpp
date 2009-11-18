#include "RisingPlatform.h"
#include "GameServices.h"
#include "TriggerEvent.h"
#include "GameEvent.h"
#include "EventManager.h"

RisingPlatform::RisingPlatform(GameServices *gs, OgreNewt::World* collisionWorld, Ogre::SceneNode *parentNode, 
					   const Ogre::Vector3 &pos, const Ogre::Vector3 &size, const Ogre::String &entityName, 
					   const Ogre::String &modelFile, const Ogre::String &listenfor)
					   :PuzzleEntity(gs, collisionWorld, parentNode, pos, size, entityName
					   , modelFile, listenfor)
{
	mRising = false;
	mFalling = false;
	mRiseSpeed = 2.0;
	mFallSpeed = 5.0;
	mInitPos = pos;
	mFinalPos = Ogre::Vector3(pos.x, pos.y+8, pos.z);
	mButtonPressed = false;
	registerListener();

	//physics copied from player
	float mMass = 10000.0f;
	Ogre::Vector3 inertia = OgreNewt::MomentOfInertia::CalcBoxSolid( mMass, mDummyNode->getScale());
	mRigidBody->setMassMatrix( mMass, inertia);
	mRigidBody->setLinearDamping(0.5f);
	mRigidBody->setAutoFreeze(0);
}
void RisingPlatform::listenCallback(GameEvent *evnt){
	mButtonPressed = true;
}
void RisingPlatform::forceCallback(OgreNewt::Body* b){
	if(mRising){
		b->setVelocity(Ogre::Vector3(0,mRiseSpeed,0));
		if(position().y > mFinalPos.y){
			b->setVelocity(Ogre::Vector3(0,0,0));
		}
	}
}
void RisingPlatform::update(){
	Ogre::Vector3 pos = position();
	Ogre::Vector3 rpos;
	Ogre::Quaternion orient;
	mRigidBody->getPositionOrientation(rpos, orient);
	setPos(Ogre::Vector3(pos.x,rpos.y,pos.z));
	mDummyNode->setOrientation(mOrient);

	if (mButtonPressed)
	{
		mRising = true;
		mFalling = false;
	}
	else{
		mRising = false;
		mFalling = true;
		setPos(mInitPos);
	}
	mButtonPressed = false;
	
	mRigidBody->setPositionOrientation(position(), mOrient);
}
void RisingPlatform::setOrientation(Ogre::Quaternion &orient){
	mOrient = orient;
}