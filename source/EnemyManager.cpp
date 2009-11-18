#include "EnemyManager.h"
#include "EnemySwarm.h"
#include "GameServices.h"

EnemyManager::EnemyManager(GameServices *gs, OgreNewt::World *collisionWorld, Ogre::SceneNode* parentNode, const Ogre::String &modelFile, Ogre::Camera* cam)
: mRandGen(mRandEngine, mRealDist)
{
	mGameServices = gs;
	mNewtonWorld = collisionWorld;
	mParentNode = parentNode;
	mModelFile = modelFile;
	mCamera = cam;
	mNextSpawnTime = 0.0f;
	mSpawnFlag = false;
	mSpawnRightSide = true;
	mTimer = new Ogre::Timer();
}

EnemyManager::~EnemyManager() {
	std::list<EnemySwarm*>::iterator swarmItr;
	for(swarmItr = mEnemySwarmList.begin(); swarmItr != mEnemySwarmList.end(); ++swarmItr) {
		delete *mEnemySwarmList.erase(swarmItr);
	}
	delete mTimer;
}

void EnemyManager::spawnEnemies(bool flag) {
	mSpawnFlag = flag;

	if(mSpawnFlag) {
		mNextSpawnTime = mRandGen();
	}
}

bool EnemyManager::isSpawning() {
	return mSpawnFlag;
}

void EnemyManager::setSwarmSpawnRate(float min, float max) {
	mSwarmSpawnRate.first = min;
	mSwarmSpawnRate.second = max;
	mRealDist = boost::uniform_real<>(min, max);
	mRandGen = boost::variate_generator<boost::minstd_rand, boost::uniform_real<> >(mRandEngine, mRealDist);
}

void EnemyManager::getSwarmSpawnRate(float &min, float &max) {
	min = mSwarmSpawnRate.first;
	max = mSwarmSpawnRate.second;
}

void EnemyManager::setEnemySpawnRate(float min, float max) {
	mEnemySpawnRate.first = min;
	mEnemySpawnRate.second = max;

	std::list<EnemySwarm*>::iterator swarmItr;
	for(swarmItr = mEnemySwarmList.begin(); swarmItr != mEnemySwarmList.end(); ++swarmItr) {
		(*swarmItr)->setSpawnRate(mEnemySpawnRate);
	}
}

void EnemyManager::getEnemySpawnRate(float &min, float &max) {
	min = mEnemySpawnRate.first;
	max = mEnemySpawnRate.second;
}

void EnemyManager::setSwarmSize(int min, int max) {
	mSwarmSize.first = min;
	mSwarmSize.second = max;
}

void EnemyManager::getSwarmSize(int &min, int &max) {
	min = mSwarmSize.first;
	max = mSwarmSize.second;
}

void EnemyManager::setTarget(Ogre::SceneNode* target) {
	mTargetNode = target;

	std::list<EnemySwarm*>::iterator swarmItr;
	for(swarmItr = mEnemySwarmList.begin(); swarmItr != mEnemySwarmList.end(); ++swarmItr) {
		(*swarmItr)->setTarget(target);
	}
}

void EnemyManager::forceWaveSpawn() {
	EnemySwarm *swarm = new EnemySwarm(mGameServices, mNewtonWorld, mParentNode, mCamera, mModelFile, mSwarmSize.second);
	swarm->setSpawnRate(mEnemySpawnRate);
	swarm->setTarget(mTargetNode);
	swarm->spawnRightSide(mSpawnRightSide);
	mSpawnRightSide = !mSpawnRightSide;
	mEnemySwarmList.push_back(swarm);
}

void EnemyManager::update() {
	if(mSpawnFlag) {
		//check to see if its time to add a swarm
		if(mTimer->getMilliseconds() / 1000.0f > mNextSpawnTime) {
			forceWaveSpawn();
			mTimer->reset();
			mNextSpawnTime = mRandGen();
		}
		
		//update the swarms and see which need to be
		//killed off.
		std::list<EnemySwarm*>::iterator swarmItr;
		std::list<std::list<EnemySwarm*>::iterator> deadSwarms;

		for(swarmItr = mEnemySwarmList.begin(); swarmItr != mEnemySwarmList.end(); ++swarmItr) {
			if((*swarmItr)->swarmSize() > 0 || (*swarmItr)->isSpawning()) {
				(*swarmItr)->update();
			} else {
				deadSwarms.push_back(swarmItr);
			}
		}
	
		//delete the dead swarms
		std::list<std::list<EnemySwarm*>::iterator>::iterator deadItr;
		for(deadItr = deadSwarms.begin(); deadItr != deadSwarms.end(); ++deadItr) {
			mEnemySwarmList.erase((*deadItr));
		}
	}
}