#include "ArenaLevel.h"
#include "GameServices.h"
#include "PlayerEntity.h"
#include "EventManager.h"
#include "RigidModelEntity.h"
#include <OgreNewt.h>
#include <Newton.h>
#include "LevelParser.h"
#include "TriggerEntity.h"
#include "CollisionHandler.h"
#include <map>
#include "PlatformCamera.h"
#include "InputManager.h"
#include "OneTimeTrigger.h"
#include "MovingBlock.h"
#include "EnemyManager.h"
#include "GameEvent.h"
#include "ArenaCamera.h"

#define Z_OFFSET -100

ArenaLevel::ArenaLevel(GameServices *gs, const Ogre::String &filePath, const Ogre::String &name) {
	mGameServices = gs;
	mInputManager = gs->input();
	mFilePath = filePath;
	mName = name;
}

ArenaLevel::~ArenaLevel() {
	unloadLevel();
}

LevelState ArenaLevel::update() {
	mPlayer1->update();
	if(mPlayer1->getHP() <= 0){
		mRunning = LS_Reset;
	}
	mPlayer2->update();
	if(mPlayer2->getHP() <= 0){
		mRunning = LS_Reset;
	}

	mFollowCam->update();

	return mRunning;
}

void ArenaLevel::loadLevel() {
	initScene();
	initLighting();
	initCamera();
	initModels();
	
	mFollowCam = new ArenaCamera(mPlayer1->getDummyNode(), mPlayer2->getDummyNode(), mCam);
	float minDist;
	mGameServices->gameConfig()->getData("arenaCamMinDist", minDist);
	mFollowCam->setMinDistanceAway(minDist);
	mLoaded = true;
	mRunning = LS_Keeper;
}

void ArenaLevel::unloadLevel() {
	mLoaded = false;

	//mGameServices->ogreRoot()->getAutoCreatedWindow()->removeAllViewports();

	mGameServices->ogreRoot()->removeFrameListener(mNewtonListener);
	delete mNewtonListener;

	//delete mCollisionWorld;
	//delete mCollisionCallback;
	//delete mPlayer1;
	//delete mPlayer2;
	//delete mArena;
	//delete mFollowCam;
	
	std::list<ModelEntity*>::iterator dynItr;
	for(dynItr = mDynamicEnts.begin(); dynItr != mDynamicEnts.end(); ++dynItr) {
		delete *dynItr;
	}
	
	mDynamicEnts.clear();

	/*std::map<std::string, ModelEntity*>::iterator staticItr;
	for(staticItr = mStaticEnts.begin(); staticItr != mStaticEnts.end(); ++staticItr) {
		delete staticItr->second;
	}*/

	mStaticEnts.clear();

	//mRootNode->removeAndDestroyAllChildren();
	mGameServices->ogreRoot()->destroySceneManager(mSceneMgr);

	

}

void ArenaLevel::initModels() {
	//PlayerEntity(GameServices *gs, OgreNewt::World *collisionWorld, Ogre::SceneNode *parentNode, Ogre::SceneNode *cameraNode, const Ogre::Vector3 &pos, const Ogre::Vector3 &size, const Ogre::String &name, const Ogre::String &modelFile);
	mPlayer1 = new PlayerEntity(mGameServices, mCollisionWorld, mPlayersNode, mCamNode, Ogre::Vector3(-50.0f, 1.0f, 0.0f), Ogre::Vector3(1.0f, 1.0f, 1.0f), "Player1", "../assets/munny.mesh");
	mPlayer2 = new PlayerEntity(mGameServices, mCollisionWorld, mPlayersNode, mCamNode, Ogre::Vector3(50.0f, 1.0f, 0.0f), Ogre::Vector3(1.0f, 1.0f, 1.0f), "Player2", "../assets/munny2.mesh");
	mPlayer1->friendlyFire(true);
	mPlayer2->friendlyFire(true);

	mArena = new ModelEntity(mGameServices, mLevelObjectsNode, Ogre::Vector3::ZERO, "Arena", "../assets/arena.mesh");
	mArena->scale(30.0f, 30.0f, 30.0f);
	mArena->position(0.0f, -100.0f, -100.0f);

	RigidModelEntity *rigidptr = new RigidModelEntity(mGameServices, mCollisionWorld, mLevelObjectsNode,
		Ogre::Vector3(0.0f, -70.0f, -240.0f+Z_OFFSET), Ogre::Vector3(30.0f, 120.0f, 30.0f), 
		"ColumnColliderN", "../assets/high_def_cube.mesh");
	ModelEntity *entptr = dynamic_cast<ModelEntity*>(rigidptr);
	addEntity( entptr, false );
	addEntity( dynamic_cast<ModelEntity*>( 
		new RigidModelEntity(mGameServices, mCollisionWorld, mLevelObjectsNode,
		Ogre::Vector3(170.0f, -70.0f, -170.0f+Z_OFFSET), Ogre::Vector3(30.0f, 100.0f, 30.0f), 
		"ColumnColliderNW", "../assets/high_def_cube.mesh") ), false);
	addEntity( dynamic_cast<ModelEntity*>( 
		new RigidModelEntity(mGameServices, mCollisionWorld, mLevelObjectsNode,
		Ogre::Vector3(-170.0f, -70.0f, -170.0f+Z_OFFSET), Ogre::Vector3(30.0f, 100.0f, 30.0f), 
		"ColumnColliderNE", "../assets/high_def_cube.mesh") ), false);
	addEntity( dynamic_cast<ModelEntity*>( 
		new RigidModelEntity(mGameServices, mCollisionWorld, mLevelObjectsNode,
		Ogre::Vector3(240.0f, -70.0f, Z_OFFSET), Ogre::Vector3(30.0f, 120.0f, 30.0f), 
		"ColumnColliderW", "../assets/high_def_cube.mesh") ), false);
	addEntity( dynamic_cast<ModelEntity*>( 
		new RigidModelEntity(mGameServices, mCollisionWorld, mLevelObjectsNode,
		Ogre::Vector3(-240.0f, -70.0f, Z_OFFSET), Ogre::Vector3(30.0f, 120.0f, 30.0f), 
		"ColumnColliderE", "../assets/high_def_cube.mesh") ), false);
	addEntity( dynamic_cast<ModelEntity*>( 
		new RigidModelEntity(mGameServices, mCollisionWorld, mLevelObjectsNode,
		Ogre::Vector3(170.0f, -70.0f, 170.0f+Z_OFFSET), Ogre::Vector3(30.0f, 100.0f, 30.0f), 
		"ColumnColliderSW", "../assets/high_def_cube.mesh") ), false);
	addEntity( dynamic_cast<ModelEntity*>( 
		new RigidModelEntity(mGameServices, mCollisionWorld, mLevelObjectsNode,
		Ogre::Vector3(-170.0f, -70.0f, 170.0f+Z_OFFSET), Ogre::Vector3(30.0f, 100.0f, 30.0f), 
		"ColumnColliderSE", "../assets/high_def_cube.mesh") ), false);
	addEntity( dynamic_cast<ModelEntity*>( 
		new RigidModelEntity(mGameServices, mCollisionWorld, mLevelObjectsNode,
		Ogre::Vector3(0.0f, -70.0f, 240.0f+Z_OFFSET), Ogre::Vector3(30.0f, 120.0f, 30.0f), 
		"ColumnColliderS", "../assets/high_def_cube.mesh") ), false);
	// floor
	addEntity( dynamic_cast<ModelEntity*>( 
		new RigidModelEntity(mGameServices, mCollisionWorld, mLevelObjectsNode,
		Ogre::Vector3(0.0f, -125.0f, 0+Z_OFFSET), Ogre::Vector3(800.0f, 50.0f, 800.0f), 
		"FloorCollider", "../assets/high_def_cube.mesh") ), false);
	// collision for exterior ring wall
	float r = 360;
	for(int i = 0; i<12; i++){
		std::string s = "WallCollider" + i;
		rigidptr = new RigidModelEntity(mGameServices, mCollisionWorld, mLevelObjectsNode,
			Ogre::Vector3(r*Ogre::Math::Sin(i*Ogre::Math::PI/6), -70.0f, -r*Ogre::Math::Cos(i*Ogre::Math::PI/6)+Z_OFFSET), 
			Ogre::Vector3(200.0f, 200.0f, 20.0f), s, "../assets/high_def_cube.mesh");
		rigidptr->rotate((-i*Ogre::Math::TWO_PI/12),Ogre::Vector3(0,1,0));
		rigidptr->updateNewtonBody();
		addEntity( dynamic_cast<ModelEntity*>( rigidptr ), false);
	}
	
	// set all the colliders invisible
	std::map<std::string,ModelEntity*>::iterator iter = mStaticEnts.begin();
	while( iter != mStaticEnts.end() )
	{
		iter->second->setVis(false);
		iter++;
	}
}

void ArenaLevel::initCamera() {
	mCam = mSceneMgr->createCamera("Camera");
	mCam->setPosition(0.0f, 200.0f, 300.0f);
	mCam->lookAt(0.0f, 0.0f, 0.0f);
	mCam->setNearClipDistance(5.0f);
	
	mCamNode->attachObject(mCam);

	Ogre::RenderWindow *win = mGameServices->ogreRoot()->getAutoCreatedWindow();

	win->removeAllViewports();	//need to remove all previously created viewports

	Ogre::Viewport *vp = win->addViewport(mCam, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f);
	vp->setBackgroundColour(Ogre::ColourValue(0.0f,0.0f,0.0f));
	mCam->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
}

void ArenaLevel::initLighting() {
	mSceneMgr->setAmbientLight( Ogre::ColourValue( 0.1f, 0.1f, 0.1f ) );	//ambient white light

	Ogre::Light *pointLight = mSceneMgr->createLight("PointLight");
	pointLight->setPosition(0.0f, 32.0f, 0.0f);
	pointLight->setType(Ogre::Light::LT_POINT);
	pointLight->setCastShadows(true);

}

void ArenaLevel::initScene() {
	mSceneMgr = mGameServices->ogreRoot()->createSceneManager(Ogre::ST_GENERIC, "SceneManager");

	mRootNode = mSceneMgr->getRootSceneNode();

	mLevelObjectsNode = mRootNode->createChildSceneNode("LevelObjects");
	mPlayersNode = mRootNode->createChildSceneNode("Players");
	mCamNode = mRootNode->createChildSceneNode("Camera", Ogre::Vector3(0.0f, 0.0f, 0.0f));
	//initialize the physics world
	mCollisionWorld = new OgreNewt::World();
	mCollisionWorld->setWorldSize(Ogre::AxisAlignedBox(-500.0f, -100.0f, -500.0f, 500.0f, 300.0f, 500.0f));
	//add the newton listener so newton can update
	mNewtonListener = new OgreNewt::BasicFrameListener(mGameServices->ogreRoot()->getAutoCreatedWindow(), mSceneMgr, mCollisionWorld, mGameServices, 120);
	
	mGameServices->ogreRoot()->addFrameListener(mNewtonListener);

	//setup materials for collisions
	const OgreNewt::MaterialID *matID = mCollisionWorld->getDefaultMaterialID();
	mMaterialPair = new OgreNewt::MaterialPair(mCollisionWorld, matID, matID);
	mCollisionCallback = new CollisionHandler(mGameServices);
	mMaterialPair->setContactCallback(mCollisionCallback);
}
void ArenaLevel::addEntity(ModelEntity *entity, bool needUpdate){
	mStaticEnts[entity->name()] = entity;	// access by name
	if( needUpdate ){	// also add copies of dynamic ents into the dynamic list
		mDynamicEnts.push_back( entity );
	}
}
int ArenaLevel::removeEntity( ModelEntity *entity, bool needUpdate){
	int n;
	n = mStaticEnts.erase( entity->name() );
	if( needUpdate ){
		std::list<ModelEntity*>::iterator result = std::find( mDynamicEnts.begin(), mDynamicEnts.end(), entity );
		mDynamicEnts.erase( result );
	}
	delete entity;
	entity = NULL;
	//entity->~ModelEntity();
	return n;	// n will 1 on success, 0 on fail
}