#include "EnemyEntity.h"
#include "GameServices.h"

EnemyEntity::EnemyEntity(GameServices *gs, OgreNewt::World *collisionWorld, Ogre::SceneNode* parentNode, const Ogre::Vector3 &pos, const Ogre::Vector3 &size, const Ogre::String &name, const Ogre::String &modelFile)
: RigidModelEntity(gs, collisionWorld, parentNode, pos, size, name, modelFile)
{
	mEntityType = ET_EnemyEntity;
	mWaitingToDie = false;

	mMass = 5.0f;
	Ogre::Vector3 inertia = OgreNewt::MomentOfInertia::CalcBoxSolid( mMass, mDummyNode->getScale());
	mRigidBody->setMassMatrix( mMass, inertia);
	mRigidBody->setLinearDamping(0.5f);
	mRigidBody->setAutoFreeze(0);

	mAnimationState = mEntity->getAnimationState("run");
	mAnimationState->setLoop(true);
	mAnimationState->setEnabled(true);

	mGameServices->gameConfig()->getData("enemyAttackDamage", mDamage);
	mGameServices->gameConfig()->getData("enemySpeed", mMaxSpeed);
	mGameServices->gameConfig()->getData("enemyLifeSpan", mLifeSpan);
	mLifeClock = new Ogre::Timer();
}

EnemyEntity::~EnemyEntity() {
	
}

void EnemyEntity::forceCallback(OgreNewt::Body* b) {
	//add gravity
	Ogre::Real mass;
	Ogre::Vector3 inertia;
	Ogre::Real gravity;
	Ogre::Vector3 pos;
	Ogre::Quaternion orient;

	b->getMassMatrix(mass, inertia);
	b->getPositionOrientation(pos,orient);
	gravity = mass * 50.0f;
	b->addForce(Ogre::Vector3(0.0f, -gravity, 0.0f));
	
	if(mTargetNode != NULL) {
		Ogre::Vector3 targetPos = mTargetNode->getPosition();
		Ogre::Vector3 enemyPos = mDummyNode->getPosition();
		Ogre::Vector3 moveDir = targetPos - enemyPos;
		moveDir.y = 0;
		moveDir.normalise();

		Ogre::Vector3 imp = moveDir * mMaxSpeed * 0.1f;
		Ogre::Vector3 vel = b->getVelocity();
		if(fabs(vel.x) >= mMaxSpeed) {
			imp.x = 0.0f;
		}
		if(fabs(vel.z) >= mMaxSpeed) {
			imp.z = 0.0f;
		}
		b->addImpulse(imp, enemyPos);
	}
}

void EnemyEntity::transformCallback(OgreNewt::Body *b, const Ogre::Quaternion& orient, const Ogre::Vector3& pos) {
	mDummyNode->setPosition(pos);

	if(mTargetNode != NULL) {
		RigidModelEntity *ent = (RigidModelEntity*)b->getUserData();
		Ogre::Vector3 lookAtPoint = mTargetNode->getPosition();
		lookAtPoint.y = mDummyNode->getPosition().y;
		ent->getDummyNode()->lookAt(lookAtPoint, Ogre::Node::TS_WORLD);
	}
}

void EnemyEntity::onCollisionEntered(RigidModelEntity *colEntity) {
	RigidModelEntity::onCollisionEntered(colEntity);
}

void EnemyEntity::kill() {
	mWaitingToDie = true;
}

bool EnemyEntity::isWaitingToDie() {
	return mWaitingToDie;
}

float EnemyEntity::attackDamage() {
	return mDamage;
}

void EnemyEntity::setTargetNode(Ogre::SceneNode *targetNode) {
	mTargetNode = targetNode;
}

void EnemyEntity::update(){
	mAnimationState->addTime(0.04);
	if(mLifeClock->getMilliseconds() > mLifeSpan * 1000.0f) {
		kill();
	}
}

