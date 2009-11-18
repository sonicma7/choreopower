#pragma once

#include <Ogre.h>
#include <OgreNewt.h>
#include <map>
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
class TwoHitMover;

class PlatformLevel : public Level{
public:
	PlatformLevel(GameServices *gs, const Ogre::String &filePath, const Ogre::String &botFilePath, const Ogre::String &name);
	virtual ~PlatformLevel();

	LevelState update();

	void portalChar( PlayerEntity* );

	void loadLevel();
	void unloadLevel();
	void getPlayers(PlayerEntity* p1, PlayerEntity* p2);
protected:
	void initScene();
	void initLighting();
	void initCamera();
	void initModels();
	void initEnemies();

	//use to add entity to container, specify if it needs to have update() run
	void addEntity( ModelEntity *entity, bool needUpdate);	 
	int removeEntity( ModelEntity *entity, bool needUpdate );
	void endLevel(GameEvent *evnt);	//end level and load arena

	GameServices *mGameServices;
	InputManager *mInputManager;
	Ogre::SceneManager *mTopSceneMgr;
	Ogre::SceneManager *mBottomSceneMgr;

	Ogre::String mBottomFilePath;
	Ogre::String mTopFilePath;
	Ogre::String mName;
	unsigned short mCountFinished;
	float mTopOffset;
	unsigned int mEntityCount;	//used to generate entity IDs, increment after creating each level object.
	std::map<std::string,ModelEntity*> mStaticEnts;	//hold all entities in the level
	std::list<ModelEntity*> mDynamicEnts; //hold only entities that get updated
	//top player
	Ogre::SceneNode *mTopRootNode;
	Ogre::SceneNode *mTopLevelObjectsNode;
	Ogre::SceneNode *mTopEnemiesNode;
	Ogre::SceneNode *mTopPlayerNode;
	
	//players
	PlayerEntity *mTopPlayer;
	PlayerEntity *mBottomPlayer;
	
	//enemies
	EnemyManager *mTopEnemyMgr;
	EnemyManager *mBottomEnemyMgr;

	//level objects
	ModelEntity *mTopFloor;
	ModelEntity *mTopBox1;
	ModelEntity *mTopBox2;
	OneTimeTrigger *mTopTrigger;
	OneTimeTrigger *mBotTrigger;
	MovingBlock	*mTopMovingBlock;
	MovingBlock *mBotMovingBlock;

	ModelEntity *mBottomFloor;
	ModelEntity *mBottomBox1;
	ModelEntity *mBottomBox2;
	RigidModelEntity *mModel;
	
	//Newton physics world
	OgreNewt::World *mTopCollisionWorld;
	OgreNewt::World *mBottomCollisionWorld;
	OgreNewt::BasicFrameListener *mTopNewtonListener;
	OgreNewt::BasicFrameListener *mBottomNewtonListener;

	//bottom player
	Ogre::SceneNode *mBottomRootNode;
	Ogre::SceneNode *mBottomLevelObjectsNode;
	Ogre::SceneNode *mBottomEnemiesNode;
	Ogre::SceneNode *mBottomPlayerNode;

	//cameras and lighting
	Ogre::Camera *mTopCam;
	Ogre::Camera *mBottomCam;
	Ogre::SceneNode *mTopCamNode;
	Ogre::SceneNode *mBottomCamNode;
	PlatformCamera *mTopCamMan;
	PlatformCamera *mBotCamMan;
	Ogre::Light *mTopPointLight;
	Ogre::Light *mBottomPointLight;

	//Newton Materials
	OgreNewt::MaterialPair *mTopMaterialPair;
	CollisionHandler *mTopCollisionCallback;
	OgreNewt::MaterialPair *mBottomMaterialPair;
	CollisionHandler *mBottomCollisionCallback;
};