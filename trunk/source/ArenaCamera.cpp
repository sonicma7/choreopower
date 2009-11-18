#include "ArenaCamera.h"

ArenaCamera::ArenaCamera(Ogre::SceneNode *target1, Ogre::SceneNode *target2, Ogre::Camera *cam) {
	mTarget1 = target1;
	mTarget2 = target2;
	mCamera = cam;
	mMinDist = 0.0f;
}

ArenaCamera::~ArenaCamera() {

}

void ArenaCamera::setMinDistanceAway(float dist) {
	mMinDist = dist;
}

void ArenaCamera::update() {
	Ogre::Vector3 lookAt;
	Ogre::Vector3 camPos(mCamera->getPosition());
	float distance;
	float idealDistance;
	float fov = mCamera->getFOVy().valueRadians();
	float xDist;
	float otherAngle = 90 - fov;

	lookAt = (mTarget1->getPosition() + mTarget2->getPosition()) * 0.5f;
	xDist = fabs(mTarget1->getPosition().x - mTarget2->getPosition().x);

	distance = camPos.z - lookAt.z;
	
	idealDistance = 0.5f * xDist * Ogre::Math::Sin(otherAngle) / Ogre::Math::Sin(0.5f * fov);
	if(idealDistance > mMinDist) {
		camPos.z = idealDistance;
	} else {
		camPos.z = mMinDist;
	}

	mCamera->setPosition(camPos);
	mCamera->lookAt(lookAt);
}