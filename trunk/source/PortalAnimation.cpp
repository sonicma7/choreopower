#include "ModelEntity.h"
#include "PortalAnimation.h"
#include "Ogre.h"
#include "InputManager.h"
#include "GameServices.h"
#include "OgreMath.h"
#include "EventManager.h"

PortalAnimation::PortalAnimation(GameServices *gs, Ogre::SceneNode *parentNode, const Ogre::Vector3 &pos, const Ogre::String &name, const Ogre::String &modelFile) 
:ModelEntity(gs, parentNode, pos, name, modelFile){
	mAnimationState = mEntity->getAnimationState("portal");
	mAnimationState->setLoop(false);
	mAnimationState->setEnabled(true);

	mDummyNode->setScale(5.0f,5.0f, 5.0f);
	mEntity->setCastShadows(false);
	Ogre::Quaternion newOrient;
	newOrient.FromAngleAxis(Ogre::Radian(Ogre::Math::HALF_PI), Ogre::Vector3(1.0f,0.0f,0.0f));
	mDummyNode->setOrientation(newOrient);
	mDummyNode->rotate(Ogre::Vector3::UNIT_Y, Ogre::Radian(Ogre::Math::HALF_PI));

	Ogre::Vector3 newPos;
	newPos = Ogre::Vector3(pos.x, pos.y + 2.0f, pos.z);
	mDummyNode->setPosition(newPos);

	mTimer = 0.0;
	complete = false;
}

PortalAnimation::~PortalAnimation(){
	delete mAnimationState;
}

Ogre::AnimationState* PortalAnimation::getAnimation(){
	return mAnimationState;
}

bool PortalAnimation::getStatus(){
	return complete;
}

void PortalAnimation::update(){
	mTimer += 0.03;
	mAnimationState->addTime(0.03);
	if(mTimer >= mAnimationState->getLength()){
		complete = true;
	}
}