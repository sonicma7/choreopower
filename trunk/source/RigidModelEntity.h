#pragma once

#include "ModelEntity.h"
#include <OgreNewt.h>
#include <Ogre.h>

class GameServices;

class RigidModelEntity : public ModelEntity {
public:
	RigidModelEntity(GameServices *gs, OgreNewt::World* collisionWorld, Ogre::SceneNode *parentNode, 
						const Ogre::Vector3 &pos, const Ogre::Vector3 &size, 
						const Ogre::String &name, const Ogre::String &modelFile);

	virtual ~RigidModelEntity();

	bool getKill();
	virtual void setKill(bool kill);
	virtual void update();
	virtual void changeCollisionWorld(OgreNewt::World *newWorld);
	virtual void setPos(const Ogre::Vector3 &pos);
	
	virtual void onCollisionEntered(RigidModelEntity *colEntity);
	virtual void onCollisionExited(RigidModelEntity *colEntity);
	virtual void forceCallback(OgreNewt::Body *b);
	virtual void transformCallback(OgreNewt::Body *b, const Ogre::Quaternion&, const Ogre::Vector3&);

	virtual void scale(float x, float y, float z);
	virtual void scale(const Ogre::Vector3 &size);
	void updateNewtonBody();
	OgreNewt::Body *mRigidBody;	//stuff needs to act on this for callbacks
	int EntityType(){return mEntityType;}

protected:
	OgreNewt::World *mNewtonWorld;
	bool mInCollision;
	bool mKill;
};