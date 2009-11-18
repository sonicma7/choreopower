#pragma once

#include <Ogre.h>

class GameServices;

enum EntityTypes {
	ET_BaseEntity = 0,
	ET_ModelEntity,
	ET_RigidModelEntity,
	ET_PlayerEntity,
	ET_EnemyEntity,
	ET_TriggetEntity,
	ET_PuzzleEntity,
	ET_WeaponEntity,
	ET_FireEntity,
	ET_GroundEntity,
	ET_WindEntity,
};

class BaseEntity {
public:
	BaseEntity(GameServices *gs, Ogre::SceneNode *parentNode, const Ogre::Vector3 &pos, const Ogre::String &entityName);
	virtual ~BaseEntity();

	virtual void update();
	
	const std::string& name() const;
	const EntityTypes getType() const;
	const Ogre::Vector3& position() const;
	virtual void position(const Ogre::Vector3 &pos);
	virtual void position(float x, float y, float z);
	virtual void translate(float x, float y, float z);
	
	const Ogre::Quaternion& orientation() const;
	void rotate(float angle, const Ogre::Vector3 &axis);
	void setVis( bool vis );
	Ogre::SceneNode* getDummyNode() const;
	//const Ogre::SceneNode* getDummyNode() const {return mDummyNode;}
	//const Ogre::SceneNode* getParentNode() const { return mDummyNode->getParent();}

	//changes the parent node and returns the old parent node
	virtual Ogre::SceneNode* changeParentNode(Ogre::SceneNode *parentNode);

protected:
	EntityTypes mEntityType;
	Ogre::SceneNode *mDummyNode;
	Ogre::SceneNode *mParentNode;
	
	Ogre::String mName;

	GameServices *mGameServices;
};
