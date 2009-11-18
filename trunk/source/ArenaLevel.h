#ifndef ARENALEVEL_H
#define ARENALEVEL_H

#include <Ogre.h>
#include <OgreNewt.h>
#include "Level.h"

class GameServices;
class InputManager;
class PlayerEntity;
class ModelEntity;
class CollisionManager;
class GameEvent;
class TriggerEntity;
class RigidModelEntity;
class CollisionHandler;
class LevelParser;
class PlatformCamera;
class OneTimeTrigger;
class MovingBlock;
class EnemyManager;
class ArenaCamera;

class ArenaLevel : public Level {
public:
	ArenaLevel(GameServices *gs, const Ogre::String &filePath, const Ogre::String &name);
	~ArenaLevel();

	LevelState update();

	void loadLevel();
	void unloadLevel();
	void getPlayers(PlayerEntity* p1, PlayerEntity* p2){p1 = mPlayer1; p2 = mPlayer2;}

protected:
	void initModels();
	void initCamera();
	void initLighting();
	void initScene();

	void addEntity( ModelEntity *entity, bool needUpdate);	 
	int removeEntity( ModelEntity *entity, bool needUpdate );
	std::map<std::string,ModelEntity*> mStaticEnts;	//hold all entities in the level
	std::list<ModelEntity*> mDynamicEnts; //hold only entities that get updated

	GameServices *mGameServices;
	InputManager *mInputManager;
	Ogre::SceneManager *mSceneMgr;
	CollisionManager *mCollisionMgr;
	Ogre::String mFilePath;
	Ogre::String mName;

	Ogre::SceneNode *mRootNode;
	Ogre::SceneNode *mLevelObjectsNode;
	Ogre::SceneNode *mPlayersNode;

	PlayerEntity *mPlayer1;
	PlayerEntity *mPlayer2;

	ModelEntity *mArena;

	Ogre::Camera *mCam;
	Ogre::SceneNode *mCamNode;
	//Newton physics world
	OgreNewt::World *mCollisionWorld;
	OgreNewt::BasicFrameListener *mNewtonListener;
	//Newton Materials
	OgreNewt::MaterialPair *mMaterialPair;
	CollisionHandler *mCollisionCallback;

	ArenaCamera *mFollowCam;
};

#endif