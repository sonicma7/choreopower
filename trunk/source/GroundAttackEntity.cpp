#include "RigidModelEntity.h"
#include "GroundAttackEntity.h"
#include "Ogre.h"
#include "InputManager.h"
#include "GameServices.h"
#include "OgreMath.h"
#include "EventManager.h"
#include "EnemyEntity.h"

GroundAttackEntity::GroundAttackEntity(GameServices *gs, OgreNewt::World *collisionWorld, Ogre::SceneNode *parentNode, const Ogre::Vector3 &pos, const Ogre::Vector3 &size, const Ogre::String &name, const Ogre::String &modelFile) 
:RigidModelEntity(gs, collisionWorld, parentNode, pos, Ogre::Vector3(1.0f), name, modelFile){
	mEntityType = ET_GroundEntity;

	mAnimationState = mEntity->getAnimationState("go");
	mAnimationState->setLoop(false);
	mAnimationState->setEnabled(true);

	mTimer = 0.0;
	complete = false;

}


GroundAttackEntity::~GroundAttackEntity(){
	delete mAnimationState;
}

Ogre::AnimationState* GroundAttackEntity::getAnimation(){
	return mAnimationState;
}

bool GroundAttackEntity::getStatus(){
	return complete;
}

void GroundAttackEntity::update(){
	mTimer += 0.04;
	mAnimationState->addTime(0.04);
	if(mTimer >= mAnimationState->getLength()){
		complete = true;
	}
}

void GroundAttackEntity::onCollisionEntered(RigidModelEntity *colEntity) {
	if(colEntity->getType() == ET_EnemyEntity) {
		((EnemyEntity*)colEntity)->kill();
	}
}