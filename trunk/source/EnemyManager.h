#include <list>
#include "OgreCamera.h"
#include "OgreSceneNode.h"
#include "OgreNewt/OgreNewt_World.h"
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/random/linear_congruential.hpp>

class GameServices;
class EnemySwarm;

class EnemyManager {
public:
	EnemyManager(GameServices *gs, OgreNewt::World *collisionWorld, Ogre::SceneNode* parentNode, const Ogre::String &modelFile, Ogre::Camera* cam);
	~EnemyManager();
	
	void spawnEnemies(bool flag);
	bool isSpawning();

	void setSwarmSpawnRate(float min, float max);
	void getSwarmSpawnRate(float &min, float &max);

	void setEnemySpawnRate(float min, float max);
	void getEnemySpawnRate(float &min, float &max);
	
	void setSwarmSize(int min, int max);
	void getSwarmSize(int &min, int &max);
	
	void setTarget(Ogre::SceneNode* target);

	void forceWaveSpawn();

	void update();

private:
	std::list<EnemySwarm*> mEnemySwarmList;
	std::pair<float, float> mEnemySpawnRate;
	std::pair<float, float> mSwarmSpawnRate;
	std::pair<int, int> mSwarmSize;
	float mNextSpawnTime;
	Ogre::Timer *mTimer;
	bool mSpawnFlag;
	bool mSpawnRightSide;

	GameServices *mGameServices;
	Ogre::Camera *mCamera;
	OgreNewt::World *mNewtonWorld;
	Ogre::SceneNode *mParentNode;
	Ogre::String mModelFile;

	Ogre::SceneNode *mTargetNode;

	//random number generation
	boost::minstd_rand mRandEngine;
	boost::uniform_real<> mRealDist;
	boost::variate_generator<boost::minstd_rand, boost::uniform_real<> > mRandGen;
};