//Munny
//This is where the Munny model is loaded and settings are changed.


#pragma once

#include <Ogre.h>
#include "ModelEntity.h"
class GameServices;

class Game;

class Munny : public ModelEntity{
public:
	Munny(GameServices *gs, Ogre::SceneNode *parentNode, const Ogre::Vector3 &pos, const Ogre::String &name, const Ogre::String &modelFile);
	~Munny();
	

private:
	Ogre::SceneNode *mMunnyNode;

};