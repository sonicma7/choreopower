#include "RigidModelEntity.h"
#include "WindAttackEntity.h"
#include "Ogre.h"
#include "InputManager.h"
#include "GameServices.h"
#include "OgreMath.h"
#include "EventManager.h"
#include "EnemyEntity.h"

WindAttackEntity::WindAttackEntity(GameServices *gs, OgreNewt::World *collisionWorld, Ogre::SceneNode *parentNode, const Ogre::Vector3 &pos, const Ogre::Vector3 &size, const Ogre::String &name, const Ogre::Radian direction, const Ogre::String &modelFile) 
:RigidModelEntity(gs, collisionWorld, parentNode, pos, size, name, modelFile){
	mEntityType = ET_WindEntity;

	mTimer = new Ogre::Timer();
	mWindSlash = 3000;
	complete = false;
	mDirection = direction;
	mDirection = mDirection.valueRadians() / fabs(mDirection.valueRadians());
	
	Ogre::Vector3 inertia = OgreNewt::MomentOfInertia::CalcBoxSolid( 10.0f, mDummyNode->getScale());
	mRigidBody->setMassMatrix( 10.0f, inertia);
	mRigidBody->setLinearDamping(0.5f);
	mRigidBody->setAutoFreeze(0);

	Ogre::Vector3 newPos;
	newPos = Ogre::Vector3(pos.x + (-mDirection.valueRadians() * 20.0f), pos.y, pos.z);
	mDummyNode->setPosition(newPos);
	setPos(newPos);
}


WindAttackEntity::~WindAttackEntity(){

}

void WindAttackEntity::forceCallback(OgreNewt::Body *b) {
	b->setVelocity(Ogre::Vector3(-150.0f * mDirection.valueRadians(), 0.0f, 0.0f));
}

void WindAttackEntity::transformCallback(OgreNewt::Body *b, const Ogre::Quaternion& orient, const Ogre::Vector3& pos) {
	Ogre::Quaternion newOrient = orient;
	newOrient.FromAngleAxis(Ogre::Radian(Ogre::Math::HALF_PI * mDirection.valueRadians()), Ogre::Vector3::UNIT_Y);
	b->setPositionOrientation(pos, newOrient);
}

bool WindAttackEntity::getStatus(){
	return complete;
}

void WindAttackEntity::update(){
	if(mTimer->getMilliseconds() >= mWindSlash){
		complete = true;
	}
	RigidModelEntity::update();
}

void WindAttackEntity::onCollisionEntered(RigidModelEntity *colEntity) {
	if(colEntity->getType() == ET_EnemyEntity) {
		((EnemyEntity*)colEntity)->kill();
	}
	if(colEntity->getType() == ET_PlayerEntity){
		complete = true;
	}
}