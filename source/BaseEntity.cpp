#include "BaseEntity.h"
#include "GameServices.h"

BaseEntity::BaseEntity(GameServices *gs, Ogre::SceneNode *parentNode, const Ogre::Vector3 &pos, const Ogre::String &entityName) {
	mGameServices = gs;

	std::ostringstream stm;
	int *temp = new int(5);
	stm << entityName << (int)temp;
	mParentNode = parentNode;
	mDummyNode = parentNode->createChildSceneNode(stm.str()+"Dummy");
	mDummyNode->setPosition(pos);

	mName = entityName;
}

BaseEntity::~BaseEntity() {
	mDummyNode->getParentSceneNode()->removeAndDestroyChild(mDummyNode->getName());
}

void BaseEntity::update() {

}

const std::string& BaseEntity::name() const {
	return mName;
}

const EntityTypes BaseEntity::getType() const {
	return mEntityType;
}

const Ogre::Vector3& BaseEntity::position() const {
	return mDummyNode->getPosition();
}

Ogre::SceneNode* BaseEntity::getDummyNode() const {
	return mDummyNode;
}

void BaseEntity::position(const Ogre::Vector3 &pos) {	
	mDummyNode->setPosition(pos);
}

void BaseEntity::position(float x, float y, float z) {
	mDummyNode->setPosition(x,y,z);
}

void BaseEntity::translate(float x, float y, float z) {
	mDummyNode->translate(x, y, z);
}

const Ogre::Quaternion& BaseEntity::orientation() const {
	return mDummyNode->getOrientation();
}

void BaseEntity::rotate(float radians, const Ogre::Vector3 &axis) {
	Ogre::Quaternion q;
	Ogre::Radian rad(radians);
	q.FromAngleAxis(rad, axis);
	mDummyNode->rotate(q);
}

Ogre::SceneNode* BaseEntity::changeParentNode(Ogre::SceneNode *parentNode) {
	Ogre::SceneNode *oldParent = mDummyNode->getParentSceneNode();
	oldParent->removeChild(mDummyNode);
	parentNode->addChild(mDummyNode);

	return oldParent;
}
void BaseEntity::setVis( bool vis ){
	mDummyNode->setVisible(vis);
}