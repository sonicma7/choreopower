#include "Munny.h"
#include "ModelEntity.h"
#include "Ogre.h"

Munny::Munny(GameServices *gs, Ogre::SceneNode *parentNode, const Ogre::Vector3 &pos, const Ogre::String &name, const Ogre::String &modelFile)
:ModelEntity(gs, parentNode, pos, name, modelFile){

}

Munny::~Munny(){

}