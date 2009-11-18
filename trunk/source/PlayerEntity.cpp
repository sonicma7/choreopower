#include "ModelEntity.h"
#include "PlayerEntity.h"
#include "WeaponEntity.h"
#include "Ogre.h"
#include "InputManager.h"
#include "GameServices.h"
#include "OgreMath.h"
#include "EventManager.h"
#include <vector>
#include "WeaponCollisionEntity.h"
#include "EnemyEntity.h"

#define ACCEL 0.3

PlayerEntity::PlayerEntity(GameServices *gs, OgreNewt::World *collisionWorld, Ogre::SceneNode *parentNode, Ogre::SceneNode *cameraNode, const Ogre::Vector3 &pos, const Ogre::Vector3 &size, const Ogre::String &name, const Ogre::String &modelFile) 
:RigidModelEntity(gs, collisionWorld, parentNode, pos, size, name, modelFile){

	mEntityType = ET_PlayerEntity;
	
	initData();

	mInputManager = gs->input();

	mCameraNode = cameraNode;
	mJumping = false;
	mStartJump = false;
	mRunning = false;
	mAttacking = false;
	mPortal = false;
	mFire = false;
	mWind = false;
	mGround = false;
	mPortalAnim = false;

	mFireBallTimer = new Ogre::Timer();
	mWindSlashTimer = new Ogre::Timer();
	mGroundSlamTimer = new Ogre::Timer();
	mFireBall = 1000;
	mWindSlash = 3000;
	mGroundSlam = 5000;

	mJumpNumber = 0;
	mMovementForce = Ogre::Vector3(0.0f);
	mMass = 10.0f;

	Ogre::Vector3 inertia = OgreNewt::MomentOfInertia::CalcBoxSolid( mMass, mDummyNode->getScale());
	mRigidBody->setMassMatrix( mMass, inertia);
	mRigidBody->setLinearDamping(0.5f);
	mRigidBody->setAutoFreeze(0);
	//mRigidBody->setContinuousCollisionMode(1);

	initAnimations();
	initWeapon();
	mHP = 100;
	mJumpTimer = new Ogre::Timer();

	mPlayerAttack = NULL;
	initHealthBar();

	mFriendlyfire = false;
}

PlayerEntity::~PlayerEntity(){
	delete mPlayerWeapon;
	delete mJumpTimer;
	delete mPlayerAttack;
}

void PlayerEntity::initData() {
	mGameServices->gameConfig()->getData("playerJumpTime", mJumpTime);
	mGameServices->gameConfig()->getData("playerJumpHeight", mJumpHeight);
	mGameServices->gameConfig()->getData("playerSpeed", mMaxSpeed);
	mGameServices->gameConfig()->getData("playerAnimSpeed", mAnimSpeed);
	mGameServices->gameConfig()->getData("playerHealth", mHP);
	mGameServices->gameConfig()->getData("playerHealth", mMaxHP);
}

void PlayerEntity::initHealthBar() {
	Ogre::Vector3 playerSize = mAABB.getSize();

	mHealthBarSet = mDummyNode->getParentSceneNode()->getCreator()->createBillboardSet(mName + "HealthBar", 1);
	mHealthBarSet->setDefaultDimensions(playerSize.z, playerSize.z * 0.2f);
	mHealthBarSet->setMaterialName("glow/green");

	Ogre::Vector3 healthBarPos(0.0f);
	healthBarPos.y = playerSize.y + playerSize.y * 0.03f;
	
	mHealthBar = mHealthBarSet->createBillboard(healthBarPos);
	mHealthBar->setColour(Ogre::ColourValue(1.0f, 0.0f, 0.0f));
	mHealthBar->setDimensions(mHealthBarSet->getDefaultWidth(), mHealthBarSet->getDefaultHeight());
	mEntityNode->attachObject(mHealthBarSet);
}

void PlayerEntity::initAnimations(){

	mAnimationBlender = new AnimationBlender(mEntity);
	mAnimationBlender->init("idle", true);

}

void PlayerEntity::initWeapon(){
	std::string weapon;
	Ogre::Vector3 pos, scale;
	Ogre::Quaternion orient;
	if(mName == "Player1") {
		weapon = "../assets/weapon_club.mesh";
		pos = Ogre::Vector3(-1.0f, 13.0f, 0.0f);
		
		Ogre::Radian angle(-Ogre::Math::PI / 2.0f);
		orient.FromAngleAxis(angle, Ogre::Vector3::UNIT_Z);
		
		scale = Ogre::Vector3(2.0f,2.0f,2.0f);
	} else {
		weapon = "../assets/weapon_wrench.mesh";
		pos = Ogre::Vector3(-1.0f, 13.0f, 3.0f);

		Ogre::Radian angle(-Ogre::Math::PI / 2.0f);
		orient.FromAngleAxis(angle, Ogre::Vector3::UNIT_Z);

		scale = Ogre::Vector3(5.0f,5.0f,3.0f);
	}

	mPlayerWeapon = new WeaponEntity(mGameServices, mDummyNode, Ogre::Vector3(0.0f), mName + "Weapon", weapon);
	mPlayerWeapon->reattachNodeToBone(mEntity, "right_hand");
	//mPlayerWeapon->detachNode();
	//mEntity->attachObjectToBone("right_hand", mPlayerWeapon->getEntity());

	//Uncomment in order to change scale and orientation of weapon on right hand
	//Can be moved to another spot (probably WeaponEntity) at a later time.
	mPlayerWeapon->getEntity()->getParentNode()->setScale(scale);
	mPlayerWeapon->getEntity()->getParentNode()->setOrientation(orient);
	mPlayerWeapon->getEntity()->getParentNode()->setPosition(pos);
}

void PlayerEntity::forceCallback(OgreNewt::Body* b) {
	//take care of gravity
	Ogre::Real mass;
	Ogre::Vector3 inertia;
	Ogre::Real gravity;
	b->getMassMatrix(mass, inertia);

	gravity = mass * 300.0f;
	b->addForce(Ogre::Vector3(0.0f, -gravity, 0.0f));
	
	//jumping
	if(mStartJump) {
		if(mJumpTimer->getMilliseconds() > mJumpTime && mJumpNumber < 2){
			mAnimationBlender->blend("jump", AnimationBlender::BlendWhileAnimating, 0.2, false);
			mJumping = true;	// useless?
			b->addForce(Ogre::Vector3(0.0f, gravity * mJumpHeight, 0.0f));
			++mJumpNumber;
			mJumpTimer->reset();
		}
		mStartJump = false;
	}

	//movement
	Ogre::Vector3 pos;
	Ogre::Quaternion orient;
	b->getPositionOrientation(pos, orient);

	Ogre::Vector3 vel(b->getVelocity());
	Ogre::Vector3 imp(0.0f);
	int xSign, zSign;
	
	(vel.x > 0.0f) ? xSign = 1 : xSign = -1;
	(vel.z > 0.0f) ? zSign = 1 : zSign = -1;

	if(fabs(vel.x) > 1.0f && mMovementForce.x == 0.0f) {
		imp.x = -xSign * mMaxSpeed * 0.01f *ACCEL;
	} else if(fabs(vel.x) < mMaxSpeed) {
		imp.x = mMaxSpeed * 0.1f * mMovementForce.x*ACCEL;
	}
	if(fabs(vel.z) > 1.0f && mMovementForce.z == 0.0f) {
		imp.z = -zSign * mMaxSpeed * 0.1f*ACCEL;
	} else if(fabs(vel.z) < mMaxSpeed) {
		imp.z = mMaxSpeed * 0.1f * mMovementForce.z*ACCEL;
	}

	b->addImpulse(imp, pos);
}

void PlayerEntity::transformCallback(OgreNewt::Body *b, const Ogre::Quaternion& orient, const Ogre::Vector3& pos) {
	Ogre::Vector3 axis(0.0f, 1.0f, 0.0f);
	
	Ogre::Radian angle = orient.getYaw();
	if(angle.valueAngleUnits() == 0.0f || mMovementForce.x == 1.0f) {
		angle = -Ogre::Math::PI / 2.0f;
	}
	else if(mMovementForce.x == -1.0f) {
		angle = Ogre::Math::PI / 2.0f;
	}
	
	Ogre::Quaternion newOrient(angle, axis);
	mRigidBody->setPositionOrientation(pos, newOrient);
}

void PlayerEntity::update() {
	mAnimate = false;

	mMovementForce = 0.0f;
	if(mName == "Player1"){
		//attack
		if((mInputManager->keyDown(OIS::KC_RSHIFT) || mInputManager->joystickButtonDown(0, 13)) &&
			!mAttacking && !mGround){
			//Blend attack animation
			mAnimationBlender->blend("swing_attack", AnimationBlender::BlendWhileAnimating, 0.2, false);
			mPlayerAttack = new WeaponCollisionEntity(mGameServices, mNewtonWorld, mDummyNode, Ogre::Vector3(0.0f, 0.0f, 0.0f), Ogre::Vector3(5.0f), mName + "Attack");
			mAttacking = true;
		}
		//fire attack
		if((mInputManager->keyReleased(OIS::KC_8) || mInputManager->joystickButtonDown(0, 11)) &&
			!mFire && !mAttacking && !mGround){
			mAnimationBlender->blend("stab_attack", AnimationBlender::BlendWhileAnimating, 0.2, false);
			mPlayerFire.push_back(new FireAttackEntity(mGameServices, mNewtonWorld, mParentNode, mDummyNode->getPosition(), Ogre::Vector3(10.0f), mName + "Fire", mDummyNode->getOrientation().getYaw()));
			mFireBallTimer->reset();
			mFire = true;
		}
		//wind attack
		/*if((mInputManager->keyReleased(OIS::KC_9) || mInputManager->joystickButtonDown(0, 10)) &&
			!mWind && !mAttacking && !mGround){
			mAnimationBlender->blend("swing_attack", AnimationBlender::BlendWhileAnimating, 0.2, false);
			mPlayerWind = new WindAttackEntity(mGameServices, mNewtonWorld, mParentNode, mDummyNode->getPosition(), Ogre::Vector3(3.0f), mName + "Wind", mDummyNode->getOrientation().getYaw());
			mWindSlashTimer->reset();
			mWind = true;
		}*/
		//ground attack
		if((mInputManager->keyReleased(OIS::KC_0) || mInputManager->joystickButtonDown(0, 9)) &&
			!mGround && !mAttacking){
			if(mJumpNumber == 0){
				mAnimationBlender->blend("hit_recover", AnimationBlender::BlendWhileAnimating, 0.2, false);
				mPlayerGround = new GroundAttackEntity(mGameServices, mNewtonWorld, mDummyNode, Ogre::Vector3(0.0f, 4.0f, 0.0f), Ogre::Vector3(5.0f), mName + "Ground");
				mGroundSlamTimer->reset();
				mGround = true;
			}
		}
		if(mGround == false){
			if(mInputManager->keyDown(OIS::KC_RIGHT) || mInputManager->joystickButtonDown(0, 5)){
				mMovementForce.x = 1.0f;
				mAnimate = true;
			}
			if(mInputManager->keyDown(OIS::KC_LEFT) || mInputManager->joystickButtonDown(0, 7)){
				mMovementForce.x = -1.0f;
				mAnimate = true;
			}
			if(mInputManager->keyDown(OIS::KC_UP) || mInputManager->joystickButtonDown(0, 4)){
				mMovementForce.z = -1.0f;
				mAnimate = true;
			}
			if(mInputManager->keyDown(OIS::KC_DOWN) || mInputManager->joystickButtonDown(0, 6)){
				mMovementForce.z = 1.0f;
				mAnimate = true;
			}
			if(mInputManager->keyPressed(OIS::KC_RETURN) || mInputManager->joystickButtonPressed(0, 14)){
				//mAnimationBlender->blend("jump", AnimationBlender::BlendWhileAnimating, 0.2, false);
				mStartJump = true;
				//mJumping = true;
				//++mJumpNumber;
			}
		}
	}
	if(mName == "Player2"){
		//attack
		if((mInputManager->keyDown(OIS::KC_LSHIFT) || mInputManager->joystickButtonDown(1, 13)) &&
			!mAttacking && !mGround){
			//Blend attack animation
			mAnimationBlender->blend("swing_attack", AnimationBlender::BlendWhileAnimating, 0.2, false);
			mPlayerAttack = new WeaponCollisionEntity(mGameServices, mNewtonWorld, mDummyNode, Ogre::Vector3(1.0f, 4.0f, 0.0f), Ogre::Vector3(5.0f), mName + "Attack");
			mAttacking = true;
		}
		//fire
		/*if((mInputManager->keyReleased(OIS::KC_1) || mInputManager->joystickButtonDown(1, 11)) &&
			!mAttacking && !mGround && !mFire){
			mAnimationBlender->blend("stab_attack", AnimationBlender::BlendWhileAnimating, 0.2, false);
			mPlayerFire.push_back(new FireAttackEntity(mGameServices, mNewtonWorld, mParentNode, mDummyNode->getPosition(), Ogre::Vector3(10.0f), mName + "Fire", mDummyNode->getOrientation().getYaw()));
			mFireBallTimer->reset();
			mFire = true;
		}*/
		//wind
		if((mInputManager->keyReleased(OIS::KC_2) || mInputManager->joystickButtonDown(1, 10)) &&
			!mAttacking && !mGround && !mWind){
			mAnimationBlender->blend("swing_attack", AnimationBlender::BlendWhileAnimating, 0.2, false);
			mPlayerWind = new WindAttackEntity(mGameServices, mNewtonWorld, mParentNode, mDummyNode->getPosition(), Ogre::Vector3(3.0f), mName + "Wind", mDummyNode->getOrientation().getYaw());
			mWindSlashTimer->reset();
			mWind = true;
		}
		//ground
		if((mInputManager->keyReleased(OIS::KC_3) || mInputManager->joystickButtonDown(1, 9)) &&
			!mAttacking && !mGround){
			if(mJumpNumber == 0){
				mAnimationBlender->blend("hit_recover", AnimationBlender::BlendWhileAnimating, 0.2, false);
				//std::ostringstream stm;
				mPlayerGround = new GroundAttackEntity(mGameServices, mNewtonWorld, mDummyNode, Ogre::Vector3(0.0f, 4.0f, 0.0f), Ogre::Vector3(5.0f), mName + "Ground");
				mGroundSlamTimer->reset();
				mGround = true;
			}
		}
		if(mGround == false){
			if(mInputManager->keyDown(OIS::KC_D) || mInputManager->joystickButtonDown(1, 5)){
				mMovementForce.x = 1.0f;
				mAnimate = true;
			}
			if(mInputManager->keyDown(OIS::KC_A) || mInputManager->joystickButtonDown(1, 7)){
				mMovementForce.x = -1.0f;
				mAnimate = true;
			}
			if(mInputManager->keyDown(OIS::KC_W) || mInputManager->joystickButtonDown(1, 4)){
				mMovementForce.z = -1.0f;
				mAnimate = true;
			}
			if(mInputManager->keyDown(OIS::KC_S) || mInputManager->joystickButtonDown(1, 6)){
				mMovementForce.z = 1.0f;
				mAnimate = true;
			}
			if(mInputManager->keyPressed(OIS::KC_SPACE) || mInputManager->joystickButtonDown(1, 14)){
				//mGameServices->debugOut("Jumps:%i, mJumping:%i, mStartJump:%i\n",mJumpNumber,mJumping,mStartJump);
				mStartJump = true;	
			}
		}
	}

	if(mAttacking && mPlayerAttack->getStatus()) {
		delete mPlayerAttack;
		mPlayerAttack = NULL;
		mAttacking = false;
	}
	
	updateAnimation();
	RigidModelEntity::update();
}

void PlayerEntity::updateAnimation(){
	//Uncomment for animations (time needs to be changed to change in evnt time)
	if(mRunning == false && mAnimate == true){
		if(!mAttacking && !mJumping && !mFire && !mWind && !mGround){

			mAnimationBlender->blend("run",AnimationBlender::BlendWhileAnimating, 0.2, true);
			mRunning = true;
		}
	}
	if(mAnimate == false && mRunning == true){
		if(!mAttacking && !mJumping && !mFire && !mWind && !mGround){

			mAnimationBlender->blend("idle", AnimationBlender::BlendWhileAnimating, 0.2, true);
			mRunning = false;
		}
	}

	if(mJumping){
		if(mAnimationBlender->complete){
			if(mAnimate){
				mAnimationBlender->blend("run",AnimationBlender::BlendWhileAnimating, 0.2, true);
			}
			else{
				mAnimationBlender->blend("idle",AnimationBlender::BlendWhileAnimating, 0.2, true);
			}
			mJumping = false;
			mAnimationBlender->addTime(0.015);
		}
	}

	if(mAttacking){
		if(mAnimationBlender->complete){
			if(mAnimate){
				mAnimationBlender->blend("run",AnimationBlender::BlendWhileAnimating, 0.2, true);
			}
			else{
				mAnimationBlender->blend("idle",AnimationBlender::BlendWhileAnimating, 0.2, true);
			}
			//mAttacking = false;
		}
	}

	if(mFire){
		if(mFireBallTimer->getMilliseconds() >= mFireBall){
			mFire = false;
		}
		if(mAnimationBlender->complete){
			if(mAnimate){
				mAnimationBlender->blend("run",AnimationBlender::BlendWhileAnimating, 0.2, true);
			}
			else{
				mAnimationBlender->blend("idle",AnimationBlender::BlendWhileAnimating, 0.2, true);
			}
		}
	}
	for(unsigned int i=0; i<mPlayerFire.size();i++){
		if(mPlayerFire[i]->getStatus()){
			delete mPlayerFire[i];
			mPlayerFire.erase(mPlayerFire.begin()+i);
		}
		else{
			mPlayerFire[i]->update();
		}
	}


	if(mWind){	
		if(mPlayerWind->getStatus()){
			delete mPlayerWind;
			mWind = false;
		}
		else{
			mPlayerWind->update();
		}
		if(mAnimationBlender->complete){
			if(mAnimate){
				mAnimationBlender->blend("run",AnimationBlender::BlendWhileAnimating, 0.2, true);
			}
			else{
				mAnimationBlender->blend("idle",AnimationBlender::BlendWhileAnimating, 0.2, true);
			}
		}
	}

	if(mGround){
		if(mPlayerGround->getStatus()){
			delete mPlayerGround;
			mGround = false;
		}
		else{
			mPlayerGround->update();
		}
		if(mAnimationBlender->complete){
			if(mAnimate){
				mAnimationBlender->blend("run",AnimationBlender::BlendWhileAnimating, 0.2, true);
			}
			else{
				mAnimationBlender->blend("idle",AnimationBlender::BlendWhileAnimating, 0.2, true);
			}
		}
	}
	if(mPortalAnim){
		if(mPlayerPortal->getStatus()){
			delete mPlayerPortal;
			mPortalAnim = false;
		}
		else{
			mPlayerPortal->update();
		}
	}

	mAnimationBlender->addTime(0.01*mAnimSpeed);

}

bool PlayerEntity::isPortal() {
	return mPortal;
}

Ogre::Vector3 PlayerEntity::getOldPos(){
	return mOldPosition;
}

void PlayerEntity::portal(OgreNewt::World *newWorld, Ogre::SceneNode *newParent, const Ogre::Vector3 &pos){
	if(mPlayerAttack != NULL) {
		delete mPlayerAttack;
		mPlayerAttack = NULL;
		mAttacking = false;
	}

	if(mPortalAnim){
		delete mPlayerPortal;
	}
	mPlayerPortal = new PortalAnimation(mGameServices, mDummyNode->getParentSceneNode(), mDummyNode->getPosition(), mName + "Portal");
	mPortalAnim = true;
	changeParentNode(newParent);
	changeCollisionWorld(newWorld);
	
	Ogre::Vector3 inertia = OgreNewt::MomentOfInertia::CalcBoxSolid( mMass, mDummyNode->getScale());
	mRigidBody->setMassMatrix( mMass, inertia);
	mRigidBody->setAutoFreeze(0);

	mOldPosition = position();
	if(mPortal == true){
		mPortal = false;
		mEntity->setCastShadows(true);
		mPlayerWeapon->getEntity()->setCastShadows(true);
	}
	else{
		mPortal = true;
		mEntity->setCastShadows(false);
		mPlayerWeapon->getEntity()->setCastShadows(false);
	}
	
	mHealthBarSet->removeBillboard(mHealthBar);
	Ogre::SceneManager *scnMgr = mDummyNode->getParentSceneNode()->getCreator();
	scnMgr->destroyMovableObject(mHealthBarSet);
	initHealthBar();
}
void PlayerEntity::onCollisionEntered(RigidModelEntity *colEntity){
	mJumpNumber = 0;
	EnemyEntity *enemy;
	if(colEntity->getKill()){
		damagePlayer(mHP);
	}
	switch(colEntity->getType()) {
	case ET_EnemyEntity:
		enemy = dynamic_cast<EnemyEntity*>(colEntity);
		damagePlayer(enemy->attackDamage());
		break;
	case ET_WeaponEntity:
		if(mFriendlyfire) {
			WeaponCollisionEntity *weapon = dynamic_cast<WeaponCollisionEntity*>(colEntity);
			if(weapon->getDummyNode()->getParentSceneNode() != mDummyNode->getParent()) {
				damagePlayer(10.0f);
			}
		}
		break;
	case ET_GroundEntity:
		if(mFriendlyfire) {
			GroundAttackEntity *ground = dynamic_cast<GroundAttackEntity*>(colEntity);
			if(ground->getDummyNode()->getParentSceneNode() != mDummyNode->getParent()) {
				damagePlayer(10.0f);
			}
		}
		break;
	case ET_WindEntity:
		if(mFriendlyfire) {
			WindAttackEntity *wind = dynamic_cast<WindAttackEntity*>(colEntity);
			damagePlayer(10.0f);
		}
		break;
	case ET_FireEntity:
		if(mFriendlyfire) {
			FireAttackEntity *fire = dynamic_cast<FireAttackEntity*>(colEntity);
			damagePlayer(10.0f);
		}
		break;
	}
}

void PlayerEntity::damagePlayer(float amount) {
	mHP -= amount;
	float height = mHealthBar->getOwnHeight();
	float width = mHealthBarSet->getDefaultWidth();

	float hpRatio = mHP / mMaxHP;
	float newWidth = width * hpRatio;
	
	Ogre::Vector3 newBarPos(mHealthBar->getPosition());
	newBarPos.z = (width - newWidth) * 0.5f;

	mHealthBar->setDimensions(newWidth, height);
	mHealthBar->setPosition(newBarPos);
}

void PlayerEntity::friendlyFire(bool flag) {
	mFriendlyfire = flag;
}