#include "IntroLevel.h"
#include "GameServices.h"
#include "CollisionHandler.h"
#include "PlayerEntity.h"
#include "StartBoxEntity.h"

IntroLevel::IntroLevel(GameServices *gs) {
	mGameServices = gs;
}

IntroLevel::~IntroLevel() {
	unloadLevel();
}

LevelState IntroLevel::update() {
	mPlayer1->update();
	mPlayer2->update();
	
	if(mStartBox->start()) {
		return LS_One;
	}

	return LS_Keeper;
}

void IntroLevel::loadLevel() {
	initScene();
	initLighting();
	initCamera();
	initModels();
}

void IntroLevel::unloadLevel() {
	//delete mInstructionsBox;
	//delete mStartBox;
	//delete mGround;
	//delete mLeftCollider;
	//delete mRightCollider;
	//delete mFrontCollider;

	mGameServices->ogreRoot()->getAutoCreatedWindow()->removeAllViewports();

	mGameServices->ogreRoot()->removeFrameListener(mNewtonListener);
	delete mNewtonListener;

	delete mCollisionWorld;

	mRootNode->removeAndDestroyAllChildren();

	mGameServices->ogreRoot()->destroySceneManager(mSceneMgr);
}

void IntroLevel::initScene() {
	mSceneMgr = mGameServices->ogreRoot()->createSceneManager(Ogre::ST_GENERIC, "IntroLevelManager");
	mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_MODULATIVE);

	mRootNode = mSceneMgr->getRootSceneNode();
	mCamNode = mRootNode->createChildSceneNode("CameraNode");
	mObjectsNode = mRootNode->createChildSceneNode("ObjectsNode");

	mCollisionWorld = new OgreNewt::World();
	mCollisionWorld->setWorldSize(Ogre::AxisAlignedBox(-500.0f, -500.0f, -500.0f, 500.0f, 500.0f, 500.0f));

	mNewtonListener = new OgreNewt::BasicFrameListener(mGameServices->ogreRoot()->getAutoCreatedWindow(), mSceneMgr, mCollisionWorld, mGameServices, 100);
	mGameServices->ogreRoot()->addFrameListener(mNewtonListener);

	//setup materials for collisions
	const OgreNewt::MaterialID *matID = mCollisionWorld->getDefaultMaterialID();
	OgreNewt::MaterialPair *mMaterialPair = new OgreNewt::MaterialPair(mCollisionWorld, matID, matID);
	mCollisionCallback = new CollisionHandler(mGameServices);
	mMaterialPair->setContactCallback(mCollisionCallback);
}

void IntroLevel::initLighting() {
	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.1f, 0.1f, 0.1));

	Ogre::Light *light = mSceneMgr->createLight("PointLight");
	light->setPosition(0.0f, 200.0f, 50.0f);
	light->setType(Ogre::Light::LT_POINT);
	light->setCastShadows(true);
	light->setAttenuation(500.0f, 0.2f, 0.0f, 0.0f);
}

void IntroLevel::initCamera() {
	mCam = mSceneMgr->createCamera("Camera");
	mCam->setPosition(Ogre::Vector3(0.0f, 50.0f, 150.0f));
	mCam->lookAt(0.0f, 50.0f, 0.0f);

	mCamNode->attachObject(mCam);

	Ogre::RenderWindow *win = mGameServices->ogreRoot()->getAutoCreatedWindow();

	win->removeAllViewports();

	Ogre::Viewport *viewport = win->addViewport(mCam);
	viewport->setBackgroundColour(Ogre::ColourValue(1,0,0));
	mCam->setAspectRatio(viewport->getActualWidth() / viewport->getActualHeight());

}

void IntroLevel::initModels() {
	mPlayer1 = new PlayerEntity(mGameServices, mCollisionWorld, mObjectsNode, mCamNode, Ogre::Vector3(-55.0f, 30.0f, 0.0f), Ogre::Vector3(1.0f), "Player1", "../assets/munny.mesh");
	mPlayer2 = new PlayerEntity(mGameServices, mCollisionWorld, mObjectsNode, mCamNode, Ogre::Vector3(55.0f, 30.0f, 0.0f), Ogre::Vector3(1.0f), "Player2", "../assets/munny2.mesh");
	
	mInstructionsBox = new RigidModelEntity(mGameServices, mCollisionWorld, mObjectsNode, Ogre::Vector3(0.0f, 60.0f, -50.0f), Ogre::Vector3(130.0f, 120.0f, 20.0f), "IntructionBox", "../assets/controls.mesh");
	mInstructionsBox->setMaterial("controls/controls");

	mGround = new RigidModelEntity(mGameServices, mCollisionWorld, mObjectsNode, Ogre::Vector3(0.0f, 0.0f, 0.0f), Ogre::Vector3(500.0f, 10.0f, 100.0f), "Ground", "../assets/high_def_cube.mesh");
	mGround->setMaterial("grass/grassM");

	mStartBox = new StartBoxEntity(mGameServices, mCollisionWorld, mObjectsNode, Ogre::Vector3(0.0f, 15.0f, 0.0f), Ogre::Vector3(20.0f, 20.0f, 20.0f), "StartBox", "../assets/start.mesh");

	mLeftCollider = new RigidModelEntity(mGameServices, mCollisionWorld, mObjectsNode, Ogre::Vector3(-70.0f, 60.0f, 0.0f), Ogre::Vector3(10.0f, 120.0f, 100.0f), "LeftCollider", "../assets/trans.mesh");
	mRightCollider = new RigidModelEntity(mGameServices, mCollisionWorld, mObjectsNode, Ogre::Vector3(70.0f, 60.0f, 0.0f), Ogre::Vector3(10.0f, 120.0f, 100.0f), "RightCollider", "../assets/trans.mesh");

	mFrontCollider = new RigidModelEntity(mGameServices, mCollisionWorld, mObjectsNode, Ogre::Vector3(0.0f, 60.0f, 50.0f), Ogre::Vector3(130.0f, 120.0f, 20.0f), "FrontCollider", "../assets/high_def_cube.mesh");
}

void IntroLevel::getPlayers(PlayerEntity* p1, PlayerEntity* p2) {
	p1 = mPlayer1;
	p2 = mPlayer2;
}