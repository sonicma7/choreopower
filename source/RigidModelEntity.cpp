#include "RigidModelEntity.h"
#include "GameServices.h"
#include "RigidModelDeletedEvent.h"
#include "EventManager.h"
#include <string>

RigidModelEntity::RigidModelEntity(GameServices *gs, OgreNewt::World* collisionWorld, Ogre::SceneNode *parentNode, 
								   const Ogre::Vector3 &pos, const Ogre::Vector3 &size, 
								   const Ogre::String &name, const Ogre::String &modelFile)
: ModelEntity(gs, parentNode, Ogre::Vector3::ZERO, name, modelFile)
{
	OgreNewt::ContactCallback();

	mEntityType = ET_RigidModelEntity;
	mRigidBody = NULL;
	mNewtonWorld = collisionWorld;
	mKill = false;

	scale(size.x, size.y, size.z);
	Ogre::Vector3 balh = mAABB.getSize();
	OgreNewt::Collision* col = new OgreNewt::CollisionPrimitives::Box(collisionWorld, balh);
	mRigidBody = new OgreNewt::Body(collisionWorld, col);
	delete col;
	
	mRigidBody->setMaterialGroupID(collisionWorld->getDefaultMaterialID());
	mRigidBody->attachToNode(mDummyNode);
	mRigidBody->setUserData(this);
	mRigidBody->setPositionOrientation(pos, Ogre::Quaternion::IDENTITY);

	boost::function<void (RigidModelEntity*, OgreNewt::Body*)> forceFunc = &RigidModelEntity::forceCallback;
	mRigidBody->setCustomForceAndTorqueCallback(forceFunc, this);
	
	boost::function<void(RigidModelEntity*, OgreNewt::Body*, const Ogre::Quaternion&, const Ogre::Vector3&)> transformFunc = &RigidModelEntity::transformCallback;
	mRigidBody->setCustomTransformCallback(transformFunc, this);
	
	mRigidBody->setContinuousCollisionMode(0);

	mInCollision = false;
}

RigidModelEntity::~RigidModelEntity() {
	RigidModelDeletedEvent *evnt = new RigidModelDeletedEvent("RigidModelDeleted", this);
	mGameServices->events()->throwEvent((GameEvent*)evnt);
	delete mRigidBody;
}

void RigidModelEntity::changeCollisionWorld(OgreNewt::World *newWorld) {
	if(newWorld != mNewtonWorld) {
		mNewtonWorld = newWorld;
		
		delete mRigidBody;
		OgreNewt::Collision *col = new OgreNewt::CollisionPrimitives::Box(newWorld, mAABB.getSize());
		mRigidBody = new OgreNewt::Body(newWorld, col);
		delete col;

		mRigidBody->setMaterialGroupID(newWorld->getDefaultMaterialID());
		mRigidBody->attachToNode(mDummyNode);
		mRigidBody->setUserData(this);
		mRigidBody->setPositionOrientation(mDummyNode->getPosition(), mDummyNode->getOrientation());

		boost::function<void (RigidModelEntity*, OgreNewt::Body*)> forceFunc = &RigidModelEntity::forceCallback;
		mRigidBody->setCustomForceAndTorqueCallback(forceFunc, this);

		boost::function<void(RigidModelEntity*, OgreNewt::Body*, const Ogre::Quaternion&, const Ogre::Vector3&)> transformFunc = &RigidModelEntity::transformCallback;
		mRigidBody->setCustomTransformCallback(transformFunc, this);
	}
}

void RigidModelEntity::update() {
	ModelEntity::update();
}

void RigidModelEntity::onCollisionEntered(RigidModelEntity *colEntity) {
	mInCollision = true;
}

void RigidModelEntity::onCollisionExited(RigidModelEntity *colEntity) {
	mInCollision = false;
}

void RigidModelEntity::forceCallback(OgreNewt::Body *b) {

}

void RigidModelEntity::transformCallback(OgreNewt::Body *b, const Ogre::Quaternion&, const Ogre::Vector3&) {

}

void RigidModelEntity::scale(float x, float y, float z) {
	Ogre::Vector3 size(x, y, z);
	Ogre::Quaternion orient(mDummyNode->getOrientation());
	Ogre::Vector3 pos(mDummyNode->getPosition());
	
	mDummyNode->setScale(x,y,z);
	mAABB.scale(Ogre::Vector3(x, y, z));
	
	if(mRigidBody != NULL) {
		OgreNewt::Collision *col = new OgreNewt::CollisionPrimitives::Box(mRigidBody->getWorld(), mAABB.getSize(), orient, pos);
		mRigidBody->setCollision(col);
		delete col;
	}
}

void RigidModelEntity::scale(const Ogre::Vector3 &size) {
	Ogre::Quaternion orient(mDummyNode->getOrientation());
	Ogre::Vector3 pos(mDummyNode->getPosition());

	mDummyNode->setScale(size);
	mAABB.scale(size);

	if(mRigidBody != NULL) {
		OgreNewt::Collision *col = new OgreNewt::CollisionPrimitives::Box(mRigidBody->getWorld(), size, orient, pos);
		mRigidBody->setCollision(col);
		delete col;
	}
}

void RigidModelEntity::setPos(const Ogre::Vector3 &pos){
	mDummyNode->setPosition(pos);
	Ogre::Quaternion orient;
	Ogre::Vector3 currentPos;
	mRigidBody->getPositionOrientation(currentPos, orient);
	orient = Ogre::Quaternion::IDENTITY;
	mRigidBody->setPositionOrientation(pos, orient);
}
void RigidModelEntity::updateNewtonBody(){
	Ogre::Quaternion orient;
	Ogre::Vector3 currentPos;
	//Ogre::Vector3 currentScale;
	orient = mDummyNode->getOrientation();
	currentPos = mDummyNode->getPosition();
	//currentPos = mDummyNode->_getDerivedPosition();
	//currentScale = mDummyNode->_getDerivedScale();
	mRigidBody->setPositionOrientation(currentPos, orient);
}

void RigidModelEntity::setKill(bool kill){
	mKill = kill;
}

bool RigidModelEntity::getKill(){
	return mKill;
}