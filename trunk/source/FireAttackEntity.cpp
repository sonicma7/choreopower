#include "RigidModelEntity.h"
#include "FireAttackEntity.h"
#include "Ogre.h"
#include "InputManager.h"
#include "GameServices.h"
#include "OgreMath.h"
#include "EventManager.h"
#include "EnemyEntity.h"

FireAttackEntity::FireAttackEntity(GameServices *gs, OgreNewt::World *collisionWorld, Ogre::SceneNode *parentNode, const Ogre::Vector3 &pos, const Ogre::Vector3 &size, const Ogre::String &name, const Ogre::Radian direction, const Ogre::String &modelFile) 
:RigidModelEntity(gs, collisionWorld, parentNode, pos, size, name, modelFile){
	mEntityType = ET_FireEntity;
	
	mTimer = new Ogre::Timer();
	mFireball = 10000;
	complete = false;
	mDirection = direction;
	mDirection = mDirection.valueRadians() / fabs(mDirection.valueRadians());
	
	Ogre::Vector3 inertia = OgreNewt::MomentOfInertia::CalcBoxSolid( 10, mDummyNode->getScale());
	mRigidBody->setMassMatrix( 10, inertia);
	mRigidBody->setLinearDamping(0.5f);
	mRigidBody->setAutoFreeze(0);

	Ogre::Vector3 newPos;
	newPos = Ogre::Vector3(pos.x + (-mDirection.valueRadians() * 20.0f), pos.y, pos.z);
	mDummyNode->setPosition(newPos);
	setPos(newPos);
}


FireAttackEntity::~FireAttackEntity(){

}

void FireAttackEntity::forceCallback(OgreNewt::Body *b) {
	b->setVelocity(Ogre::Vector3(-150.0f * mDirection.valueRadians(), 0.0f, 0.0f));
}

void FireAttackEntity::transformCallback(OgreNewt::Body *b, const Ogre::Quaternion& orient, const Ogre::Vector3& pos) {
	b->setPositionOrientation(pos, orient);
}

bool FireAttackEntity::getStatus(){
	return complete;
}

void FireAttackEntity::update(){
	if(mTimer->getMilliseconds() >= mFireball){
		complete = true;
	}
	RigidModelEntity::update();
}

void FireAttackEntity::onCollisionEntered(RigidModelEntity *colEntity){
	complete = true;
	if(colEntity->getType() == ET_EnemyEntity) {
		((EnemyEntity*)colEntity)->kill();
	}
}