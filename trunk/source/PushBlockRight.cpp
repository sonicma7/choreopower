#include "PushBlockRight.h"
#include "GameServices.h"
#include "TriggerEvent.h"
#include "GameEvent.h"
#include "EventManager.h"
#include "PressureSensor.h"

PushBlockRight::PushBlockRight(GameServices *gs, OgreNewt::World* collisionWorld, Ogre::SceneNode *parentNode, 
							   const Ogre::Vector3 &pos, const Ogre::Vector3 &size, const Ogre::String &entityName, 
							   const Ogre::String &modelFile, const Ogre::String &triggername, 
							   const Ogre::String &listenfor, const int triggerID, const Ogre::Vector3 &dest,
							   float pushSpeed)
							   : PuzzleEntity(gs, collisionWorld, parentNode, pos, size, entityName, modelFile,
							   triggername, listenfor, triggerID)
{
	mDest = dest;
	mSpeed = pushSpeed;
	mBump = false;
	mThrowsTriggers = false;
	mSize = size;
	mSensor = new PressureSensor(gs, collisionWorld, mDummyNode,
		Ogre::Vector3(pos.x-size.x/2,pos.y,pos.z), Ogre::Vector3(size.x/10,size.y,size.z), 
		//Ogre::Vector3(-size.x/40, 0, 0), Ogre::Vector3(size.x/50,size.y/50,size.z/50),
		entityName+"_Sensor", "../assets/high_def_cube.mesh", triggername, triggerID);
	registerListener();
	mSensor->setVis(true);
	//physics copied from player
	float mMass = 1000.0f;
	Ogre::Vector3 inertia = OgreNewt::MomentOfInertia::CalcBoxSolid( mMass, mDummyNode->getScale());
	mRigidBody->setMassMatrix( mMass, inertia);
	mRigidBody->setLinearDamping(0.5f);
	mRigidBody->setAutoFreeze(0);
}
void PushBlockRight::listenCallback(GameEvent *evnt){
	// move to the right
	//mGameServices->debugOut("PuzzleEntity received trigger event: %s", mListenFor.c_str());
	Ogre::Vector3 currPos = this->position();
	Ogre::Vector3 sensPos = mSensor->position();
	if (currPos.x < mDest.x){
		this->setPos(Ogre::Vector3(currPos.x+mSpeed,currPos.y,currPos.z));
		mSensor->setPos(Ogre::Vector3(sensPos.x+mSpeed,sensPos.y,sensPos.z));
	}
	mBump = true;
}
void PushBlockRight::forceCallback(OgreNewt::Body* b){
	if(mBump){
		b->addForce(Ogre::Vector3(5000000,0,0));
		mBump = false;
	}
	RigidModelEntity::update();
}
void PushBlockRight::update(){
	Ogre::Vector3 pos = position();
	Ogre::Vector3 rpos;
	Ogre::Quaternion orient;
	mRigidBody->getPositionOrientation(rpos, orient);
	setPos(Ogre::Vector3(rpos.x,pos.y,pos.z));
	mSensor->setPos(Ogre::Vector3(rpos.x-mSize.x/2,pos.y,pos.z));
	RigidModelEntity::update();
	//mGameServices->debugOut("updating");
}