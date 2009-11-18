#pragma once

#include <Ogre.h>
#include <OgreNewt.h>
#include "RigidModelEntity.h"
#include "WeaponEntity.h"
#include "AnimationBlender.h"
#include "FireAttackEntity.h"
#include "WindAttackEntity.h"
#include "GroundAttackEntity.h"
#include "PortalAnimation.h"
#include <vector>

class GameServices;
class InputManager;
class WeaponCollisionEntity;

class PlayerEntity : public RigidModelEntity {
public:
	PlayerEntity(GameServices *gs, OgreNewt::World *collisionWorld, Ogre::SceneNode *parentNode, Ogre::SceneNode *cameraNode, const Ogre::Vector3 &pos, const Ogre::Vector3 &size, const Ogre::String &name, const Ogre::String &modelFile);
	~PlayerEntity();

	virtual void update();
	bool isPortal();
	
	void portal(OgreNewt::World *newWorld, Ogre::SceneNode *newParent, const Ogre::Vector3 &pos);

	Ogre::Vector3 getOldPos();
	unsigned int getHP(){return mHP;}
	void setHP(unsigned int hp){mHP = hp;}
	//callbacks for the physics engine
	
	//called when force is ready to be applied. Put all
	//force related code in here
	virtual void forceCallback(OgreNewt::Body* b);

	//when the body is being moved and rotated, the final
	//position and rotation can be modified in this function.
	virtual void transformCallback(OgreNewt::Body *b, const Ogre::Quaternion&, const Ogre::Vector3&);
	void onCollisionEntered(RigidModelEntity *colEntity);
	
	void damagePlayer(float amount);
	
	void friendlyFire(bool flag);

protected:
	void initData();
	void initHealthBar();
	void initAnimations();
	void initWeapon();

	void moveRight();
	void moveLeft();
	void moveUp();
	void moveDown();
	void updateAnimation();

	OgreNewt::ContactCallback *call;
	
	AnimationBlender *mAnimationBlender;
	WeaponEntity *mPlayerWeapon;

	//Each player's special attacks
	std::vector<FireAttackEntity*> mPlayerFire;
	WindAttackEntity *mPlayerWind;
	GroundAttackEntity *mPlayerGround;
	WeaponCollisionEntity *mPlayerAttack;
	
	PortalAnimation *mPlayerPortal;
	
	float mFireBall;
	float mWindSlash;
	float mGroundSlam;

	Ogre::Timer *mWindSlashTimer;
	Ogre::Timer *mFireBallTimer;
	Ogre::Timer *mGroundSlamTimer;
	
	Ogre::SceneNode *mWeaponNode;
	InputManager *mInputManager;
	Ogre::SceneNode *mCameraNode;
	Ogre::Vector3 mMovementForce;
	Ogre::AnimationState *mAnimationState;

	//Bools for animation blender
	bool mPortalAnim;
	bool mJumping;
	bool mStartJump;
	bool mRunning;
	bool mAttacking;
	bool mAnimate;
	bool mPortal;
	bool mFire;
	bool mGround;
	bool mWind;

	Ogre::Vector3 mOldPosition;
	int mJumpNumber;
	float mMaxSpeed;
	float mMass;
	// timers
	Ogre::Timer *mJumpTimer;
	// Player state data to save
	float mHP;
	float mMaxHP;
	unsigned int mJumpTime;
	float mJumpHeight;
	float mAnimSpeed;

	//health bar stuff
	Ogre::BillboardSet *mHealthBarSet;
	Ogre::Billboard *mHealthBar;

	bool mFriendlyfire;
};