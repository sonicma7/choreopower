#pragma once

#include <Ogre.h>
#include "BaseEntity.h"
class GameServices;

class ModelEntity : public BaseEntity {
public:
	ModelEntity(GameServices *gs, Ogre::SceneNode *parentNode, const Ogre::Vector3 &pos, const Ogre::String &name, const Ogre::String &modelFile);
	virtual ~ModelEntity();

	virtual void update();
	Ogre::Entity* getEntity();

	void setMaterial(std::string materialFile);
	
	virtual const Ogre::Vector3& scale() const;
	virtual void scale(float x, float y, float z);
	virtual void scale(const Ogre::Vector3 &size);

	virtual Ogre::SceneNode* changeParentNode(Ogre::SceneNode *parentNode);

protected:
	Ogre::SceneNode *mEntityNode;
	Ogre::AxisAlignedBox mAABB;
	Ogre::Entity *mEntity;
};