#include "ModelEntity.h"
#include "BaseEntity.h"
#include "GameServices.h"

ModelEntity::ModelEntity(GameServices *gs, Ogre::SceneNode *parentNode, const Ogre::Vector3 &pos, const Ogre::String &name, const Ogre::String &modelFile) 
:BaseEntity(gs, parentNode, pos, name)
{
	std::ostringstream stm;
	int *temp = new int(5);
	stm << name << (int)temp;
	mEntity = parentNode->getCreator()->createEntity(stm.str(), modelFile);
	mAABB = mEntity->getBoundingBox();
	mEntityNode = mDummyNode->createChildSceneNode(stm.str());
	mEntityNode->attachObject(mEntity);
	mEntity->setCastShadows(true);

}

ModelEntity::~ModelEntity() {
	mEntityNode->detachObject(mEntity);
	//mDummyNode->getCreator()->destroyMovableObject(mEntity);
	delete mEntityNode;
}

Ogre::Entity* ModelEntity::getEntity(){
	return mEntity;
}

void ModelEntity::update() {
	BaseEntity::update();

}

void ModelEntity::setMaterial(std::string materialFile){
	mEntity->setMaterialName(materialFile);
}

const Ogre::Vector3& ModelEntity::scale() const {
	return mEntityNode->getScale();
}

void ModelEntity::scale(float x, float y, float z) {
	mDummyNode->setScale(x, y, z);
}

void ModelEntity::scale(const Ogre::Vector3 &size) {
	mDummyNode->setScale(size);
}

Ogre::SceneNode* ModelEntity::changeParentNode(Ogre::SceneNode *parentNode) {
	Ogre::SceneNode *oldParent = mDummyNode->getParentSceneNode();

	//if the nodes have the same scene managers, then everything is alright.
	//if the nodes have different scene managers, the node must be recreated
	//under the new scene manager.
	if(parentNode->getCreator() == mDummyNode->getCreator()) {	
		oldParent->removeChild(mDummyNode);
		parentNode->addChild(mDummyNode);
	} else {
		Ogre::String name = mDummyNode->getName();
		Ogre::Vector3 pos = mDummyNode->getPosition();
		Ogre::Quaternion orient = mDummyNode->getOrientation();
		Ogre::Vector3 scale = mDummyNode->getScale();

		oldParent->removeAndDestroyChild(name);
		mDummyNode = parentNode->createChildSceneNode(name, pos, orient);
		mDummyNode->setScale(scale);

		mEntityNode = mDummyNode->createChildSceneNode();
		mEntityNode->attachObject(mEntity);
	}

	return oldParent;
}