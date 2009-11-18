#include "RigidModelEntity.h"

class EnemyEntity : public RigidModelEntity {
public:
	EnemyEntity(GameServices *gs, OgreNewt::World *collisionWorld, Ogre::SceneNode* parentNode, const Ogre::Vector3 &pos, const Ogre::Vector3 &size, const Ogre::String &name, const Ogre::String &modelFile);
	virtual ~EnemyEntity();
	
	//called when force is ready to be applied. Put all
	//force related code in here
	virtual void forceCallback(OgreNewt::Body* b);
	virtual void transformCallback(OgreNewt::Body *b, const Ogre::Quaternion&, const Ogre::Vector3&);

	virtual void onCollisionEntered(RigidModelEntity *colEntity);
	
	void kill();
	float attackDamage();
	bool isWaitingToDie();
	void setTargetNode(Ogre::SceneNode *targetNode);
	void update();

private:
	bool mWaitingToDie;
	float mMass;
	float mMaxSpeed;
	Ogre::SceneNode *mTargetNode;
	Ogre::AnimationState *mAnimationState;
	float mDamage;
	float mLifeSpan;
	Ogre::Timer *mLifeClock;
};