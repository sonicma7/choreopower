#include "ModelEntity.h"
#include "WeaponEntity.h"
#include "Ogre.h"
#include "InputManager.h"
#include "GameServices.h"
#include "OgreMath.h"
#include "EventManager.h"

WeaponEntity::WeaponEntity(GameServices *gs, Ogre::SceneNode *parentNode, const Ogre::Vector3 &pos, const Ogre::String &name, const Ogre::String &modelFile) 
:ModelEntity(gs, parentNode, pos, name, modelFile){

	mEntityType = ET_WeaponEntity;
}


WeaponEntity::~WeaponEntity(){

}

void WeaponEntity::reattachNodeToBone(Ogre::Entity *ent, const Ogre::String &boneName) {
	mEntityNode->detachAllObjects();
	ent->attachObjectToBone(boneName, mEntity);
}