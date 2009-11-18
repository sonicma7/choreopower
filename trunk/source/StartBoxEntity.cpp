#include "StartBoxEntity.h"

StartBoxEntity::StartBoxEntity(GameServices *gs, OgreNewt::World *collisionWorld, Ogre::SceneNode *parentNode, const Ogre::Vector3 &pos, const Ogre::Vector3 &size, const Ogre::String &name, const Ogre::String &modelFile) 
:RigidModelEntity(gs, collisionWorld, parentNode, pos, size, name, modelFile) {
	mStart = false;
}

StartBoxEntity::~StartBoxEntity() {

}

void StartBoxEntity::update() {

}

bool StartBoxEntity::start() {
	return mStart;
}

void StartBoxEntity::onCollisionEntered(RigidModelEntity *colEntity) {
	mStart = true;
}