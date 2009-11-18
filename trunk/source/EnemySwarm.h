#include <list>
#include "OgreTimer.h"
#include "OgreSceneNode.h"
#include "OgreNewt/OgreNewt_World.h"
#include <boost/random/linear_congruential.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>

class EnemyEntity;
class GameServices;

class EnemySwarm {
public:
	EnemySwarm(GameServices *gs, OgreNewt::World *collisionWorld, Ogre::SceneNode *parentNode, Ogre::Camera *cam, const Ogre::String &modelFile, int size);
	~EnemySwarm();

	unsigned int swarmSize();
	
	void setSpawnRate(const std::pair<float, float> &rate);
	void getSpawnRate(float &min, float &max);
	void spawnRightSide(bool flag);
	void setTarget(Ogre::SceneNode *target);

	void update();
	
	bool isSpawning();

private:
	void calcCameraExtents(float depth, Ogre::Vector3 &left, Ogre::Vector3 &right);

	std::list<EnemyEntity*> mEnemyList;
	std::pair<float, float> mSpawnRate;
	Ogre::Timer *mTimer;
	float mNextSpawnTime;
	unsigned int mMaxSize;
	unsigned int mNumSpawned;
	bool mSpawnRightSide;

	GameServices *mGameServices;
	OgreNewt::World *mNewtonWorld;
	Ogre::SceneNode *mParentNode;
	Ogre::Camera *mCamera;
	Ogre::String mModelFile;
	
	Ogre::SceneNode *mTargetNode;

	//random numbers
	boost::minstd_rand mRandEngine;
	boost::uniform_real<> mRandRealDist;
	boost::variate_generator<boost::minstd_rand, boost::uniform_real<> > mRandGen;
};