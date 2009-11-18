#include "Level.h"
#include <Ogre.h>
#include <OgreNewt.h>

class GameServices;
class CollisionHandler;
class PlayerEntity;
class StartBoxEntity;

class IntroLevel : public Level {
public:
	IntroLevel(GameServices *gs);
	~IntroLevel();

	LevelState update();

	void loadLevel();
	void unloadLevel();

	void getPlayers(PlayerEntity* p1, PlayerEntity* p2);
private:
	void initScene();
	void initLighting();
	void initCamera();
	void initModels();

	Ogre::SceneManager *mSceneMgr;
	Ogre::SceneNode *mRootNode;
	Ogre::SceneNode *mObjectsNode;
	Ogre::SceneNode *mCamNode;

	RigidModelEntity *mInstructionsBox;
	StartBoxEntity *mStartBox;
	RigidModelEntity *mGround;
	RigidModelEntity *mLeftCollider;
	RigidModelEntity *mRightCollider;
	RigidModelEntity *mFrontCollider;

	OgreNewt::World *mCollisionWorld;
	OgreNewt::BasicFrameListener *mNewtonListener;
	OgreNewt::MaterialPair *mMaterialPair;

	Ogre::Camera *mCam;
	CollisionHandler *mCollisionCallback;
	
	GameServices *mGameServices;

	PlayerEntity *mPlayer1;
	PlayerEntity *mPlayer2;
};