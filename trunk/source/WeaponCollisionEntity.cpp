#include "RigidModelEntity.h"
#include "WeaponCollisionEntity.h"
#include "Ogre.h"
#include "InputManager.h"
#include "GameServices.h"
#include "OgreMath.h"
#include "EventManager.h"
#include "EnemyEntity.h"

WeaponCollisionEntity::WeaponCollisionEntity(GameServices *gs, OgreNewt::World *collisionWorld, Ogre::SceneNode *parentNode, const Ogre::Vector3 &pos, const Ogre::Vector3 &size, const Ogre::String &name, const Ogre::String &modelFile) 
:RigidModelEntity(gs, collisionWorld, parentNode, parentNode->_getDerivedPosition(), Ogre::Vector3(1.0f), name, modelFile){
	mEntityType = ET_WeaponEntity;

	mTimer = new Ogre::Timer();
	mEntity->setVisible(false);
}


WeaponCollisionEntity::~WeaponCollisionEntity(){
}

void WeaponCollisionEntity::onCollisionEntered(RigidModelEntity *colEntity) {
	if(colEntity->getType() == ET_EnemyEntity) {
		((EnemyEntity*)colEntity)->kill();
	}
}

bool WeaponCollisionEntity::getStatus(){
	return mTimer->getMilliseconds() / 1000.0f > 2.0f;
}