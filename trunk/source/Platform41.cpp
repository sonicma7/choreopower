#include "Platform41.h"
#include "GameServices.h"
#include "TriggerEvent.h"
#include "GameEvent.h"
#include "EventManager.h"

Platform41::Platform41(GameServices *gs, OgreNewt::World* collisionWorld, Ogre::SceneNode *parentNode, 
					   const Ogre::Vector3 &pos, const Ogre::Vector3 &size, const Ogre::String &entityName, 
					   const Ogre::String &modelFile, const Ogre::String &listenfor)
					   :PuzzleEntity(gs, collisionWorld, parentNode, pos, size, entityName
					   , modelFile, listenfor)
{
	mActive = false;
	mLockX = true;
	mLockY = true;
	mLockZ = true;
	mPressed1A = false;
	mPressed1B = false;
	mPressed2 = false;
	mPressed3 = false;
	mInPhase1 = true;
	mInPhase2 = false;
	mInPhase3 = false;
	mSpeed = 10.0;
	mDest1 = Ogre::Vector3(pos.x+85, pos.y, pos.z);
	mDest2 = Ogre::Vector3(mDest1.x,mDest1.y,mDest1.z-55);
	mDest3 = Ogre::Vector3(mDest2.x+770,mDest2.y,mDest2.z);
	//registerListener();
	mListenFor2 = "Trigger_Platform41_Phase2";
	mListenFor3 = "Trigger_Platform41_Phase3";
	mGameServices->events()->addListener(mListenFor2, this, &Platform41::listenCallback);
	mGameServices->events()->addListener(mListenFor3, this, &Platform41::listenCallback);
	mGameServices->events()->addListener(mListenFor+"_A", this, &Platform41::listenCallback);
	mGameServices->events()->addListener(mListenFor+"_B", this, &Platform41::listenCallback);

	//physics copied from player
	float mMass = 10000.0f;
	Ogre::Vector3 inertia = OgreNewt::MomentOfInertia::CalcBoxSolid( mMass, mDummyNode->getScale());
	mRigidBody->setMassMatrix( mMass, inertia);
	mRigidBody->setLinearDamping(0.5f);
	mRigidBody->setAutoFreeze(0);
}
void Platform41::listenCallback(GameEvent *evnt){
	//mGameServices->debugOut("Platform41 received trigger event: %s", evnt->name().c_str());
	if (evnt->name() == mListenFor+"_A"){
		mPressed1A = true;
		//mGameServices->debugOut("Switch1A pressed");
	}
	if (evnt->name() == mListenFor+"_B"){
		mPressed1B = true;
		//mGameServices->debugOut("Switch1B pressed");
	}
	if (evnt->name() == mListenFor2) {
		mPressed2 = true;
		//mGameServices->debugOut("Switch2 pressed");
	}
	if (evnt->name() == mListenFor3){
		mPressed3 = true;
		//mGameServices->debugOut("Switch3 pressed");
	}
}
void Platform41::forceCallback(OgreNewt::Body* b){
	if(mActive)
	{
		if(mInPhase1){
			b->setVelocity(Ogre::Vector3(mSpeed,0,0));
			if(position().x > mDest1.x){
				//mGameServices->debugOut("Dest1 reached!");
				b->setVelocity(Ogre::Vector3(0,0,0));
				setPos(mDest1);
				updateNewtonBody();
				mLockX = true;
				mInPhase1 = false;
				mInPhase2 = true;
				mGameServices->events()->removeListener(mListenFor+"_A", this);
				mGameServices->events()->removeListener(mListenFor+"_B", this);
			}
		}
		if(mInPhase2){
			mLockZ = false;
			//mLockX = false;
			if(mPressed2){
				b->setVelocity(Ogre::Vector3(0,0,-mSpeed));
				if(position().z < mDest2.z){
					//mGameServices->debugOut("Dest2 reached!");
					b->setVelocity(Ogre::Vector3(0,0,0));
					mLockZ = true;
					//mLockX = false;
					//mActive = false;
					mInPhase2 = false;
					mInPhase3 = true;
					mPressed3 = true;
					setPos(mDest2);
					updateNewtonBody();
					mGameServices->events()->removeListener(mListenFor2, this);
				}
			}
		}
		if(mInPhase3){
			mLockX = false;
			if(mPressed3){
				//mGameServices->debugOut("phase3");
				b->setVelocity(Ogre::Vector3(mSpeed,0,0));
				if(position().x > mDest3.x){
					//mGameServices->debugOut("Dest3 reached!");
					b->setVelocity(Ogre::Vector3(0,0,0));
					//setPos(mDest1);
					//updateNewtonBody();
					//mLockX = true;
					mInPhase3 = false;
					mInPhase2 = false;
					mGameServices->events()->removeListener(mListenFor3, this);
				}
			}
		}
	}
}
void Platform41::update(){
	if (mPressed1A && mPressed1B)
	{
		mActive = true;
		mLockX = false;
	}
	else
	{
		mPressed1A = false;
		mPressed1B = false;
	}
	//mGameServices->debugOut("updating");
	Ogre::Vector3 pos = position();
	Ogre::Vector3 rpos;
	Ogre::Quaternion orient;
	mRigidBody->getPositionOrientation(rpos, orient);
	if(!mLockX){
		//mGameServices->debugOut("updating X: %f", rpos.x);
		setPos(Ogre::Vector3(rpos.x,pos.y,pos.z));
	}
	if(!mLockY){
		//mGameServices->debugOut("updating Y");
		setPos(Ogre::Vector3(pos.x,rpos.y,pos.z));
	}
	if(!mLockZ){
		setPos(Ogre::Vector3(pos.x,pos.y,rpos.z));
	}
	//setPos(Ogre::Vector3(rpos.x,rpos.y,rpos.z));
	mDummyNode->setOrientation(mOrient);
	mRigidBody->setPositionOrientation(position(), mOrient); //bring the newtonBody back into position
}
void Platform41::setOrientation(Ogre::Quaternion &orient){
	mOrient = orient;
}