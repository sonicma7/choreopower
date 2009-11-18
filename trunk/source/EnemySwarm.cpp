#include "EnemySwarm.h"
#include "EnemyEntity.h"
#include "GameServices.h"

EnemySwarm::EnemySwarm(GameServices *gs, OgreNewt::World *collisionWorld, Ogre::SceneNode* parentNode, Ogre::Camera *cam, const Ogre::String &modelFile, int size)
: mRandGen(mRandEngine, mRandRealDist)
{
	mGameServices = gs;
	mNewtonWorld = collisionWorld;
	mParentNode = parentNode;
	mCamera = cam;
	mModelFile = modelFile;
	mMaxSize = size;
	mNumSpawned = 0;
	mNextSpawnTime = 0.0f;
	mTimer = new Ogre::Timer();

	mTargetNode = NULL;
}

EnemySwarm::~EnemySwarm() {

}

unsigned int EnemySwarm::swarmSize() {
	return mEnemyList.size();
}

void EnemySwarm::setSpawnRate(const std::pair<float, float> &rate) {
	mSpawnRate = rate;
	mRandRealDist = boost::uniform_real<>(rate.first, rate.second);
	mRandGen = boost::variate_generator<boost::minstd_rand, boost::uniform_real<> >(mRandEngine, mRandRealDist);
}

void EnemySwarm::getSpawnRate(float &min, float &max) {
	min = mSpawnRate.first;
	max = mSpawnRate.second;
}

void EnemySwarm::spawnRightSide(bool flag) {
	mSpawnRightSide = flag;
}

void EnemySwarm::setTarget(Ogre::SceneNode *target) {
	mTargetNode = target;
	std::list<EnemyEntity*>::iterator itr;
	for(itr = mEnemyList.begin(); itr != mEnemyList.end(); ++itr) {
		(*itr)->setTargetNode(target);
	}
}

void EnemySwarm::update() {
	if(mNumSpawned < mMaxSize) {
		if(mTimer->getMilliseconds() / 1000.0f > mNextSpawnTime) {
			std::ostringstream stm;
			stm << (int)this << mNumSpawned;
			std::string name("Enemy" + stm.str());
			
			Ogre::Vector3 leftExt, rightExt, pos;
			if(mTargetNode != NULL) {
				float depth = fabs(mTargetNode->getPosition().z - mCamera->getPosition().z);
				calcCameraExtents(depth, leftExt, rightExt);

				mSpawnRightSide ? pos = rightExt : pos = leftExt;
				EnemyEntity *enemy = new EnemyEntity(mGameServices, mNewtonWorld, mParentNode,
													pos, Ogre::Vector3(0.75f), name, 
													mModelFile);
				enemy->setTargetNode(mTargetNode);
				mEnemyList.push_back(enemy);
			}

			mTimer->reset();
			mNextSpawnTime = mRandGen();

			++mNumSpawned;
		}
	}

	std::list<EnemyEntity*>::iterator enemyItr;
	std::list<std::list<EnemyEntity*>::iterator> toDelete;
	for(enemyItr = mEnemyList.begin(); enemyItr != mEnemyList.end(); ++enemyItr) {
		if(!(*enemyItr)->isWaitingToDie()) {
			(*enemyItr)->update();
		} else {
			toDelete.push_back(enemyItr);
		}
	}
	
	std::list<std::list<EnemyEntity*>::iterator>::iterator itr;
	for(itr = toDelete.begin(); itr != toDelete.end(); ++itr) {
		EnemyEntity *e = (**itr);
		mEnemyList.erase(*itr);
		delete e;
	}
}

bool EnemySwarm::isSpawning() {
	return mNumSpawned != mMaxSize;
}

void EnemySwarm::calcCameraExtents(float depth, Ogre::Vector3 &left, Ogre::Vector3 &right) {
	float aspectRatio = mCamera->getViewport()->getHeight() / mCamera->getViewport()->getWidth();
	float fov = mCamera->getFOVy().valueRadians();
	fov *= 2.0f;
	float oppositeAngle = 90.0f - fov;

	float length = depth * Ogre::Math::Sin(fov * 0.5f) / Ogre::Math::Sin(oppositeAngle);

	left.x = mCamera->getDerivedPosition().x - length;
	right.x = mCamera->getDerivedPosition().x + length;
	right.y = left.y = mTargetNode->getPosition().y;
	right.z = left.z = mTargetNode->getPosition().z;
}