#pragma once

#include "RigidModelEntity.h"
#include <OgreNewt.h>
#include <Ogre.h>
#include "GameServices.h"
class GameServices;
class TriggerEvent;

class TriggerEntity : public RigidModelEntity {
public:
	TriggerEntity(GameServices *gs, OgreNewt::World* collisionWorld, Ogre::SceneNode *parentNode, 
					const Ogre::Vector3 &pos, const Ogre::Vector3 &size, 
					const Ogre::String &name, const Ogre::String &modelFile, 
					const int triggerID);
	
	virtual ~TriggerEntity();
	//virtual void update();
	virtual void forceCallback(OgreNewt::Body *b);
	virtual void throwTrigger();
	virtual void onCollisionEntered(RigidModelEntity *colEntity);
	virtual void onCollisionExited(RigidModelEntity *colEntity);

protected:
	const int mTriggerID;
	const Ogre::SceneNode *mOtherNode;	//delete this
};