#include "PlatformLevel.h"
#include "GameServices.h"
#include "PlayerEntity.h"
#include "EventManager.h"
#include "RigidModelEntity.h"
#include <OgreNewt.h>
#include <Newton.h>
#include "LevelParser.h"
#include "TriggerEntity.h"
#include "CollisionHandler.h"
#include <vector>
#include <map>
#include "PlatformCamera.h"
#include "InputManager.h"
#include "OneTimeTrigger.h"
#include "MovingBlock.h"
#include "EnemyManager.h"
#include "GameEvent.h"
#include "TwoHitMover.h"
#include "HazardTrigger.h"
#include "PushBlockRight.h"
#include "Platform41.h"
#include "PressureSensor.h"
#include "RisingPlatform.h"
#include "Drawbridge.h"

PlatformLevel::PlatformLevel(GameServices *gs, const Ogre::String &filePath, const Ogre::String &botFilePath, const Ogre::String &name) {
	mGameServices = gs;
	mInputManager = gs->input();
	mTopFilePath = filePath;
	mBottomFilePath = botFilePath;
	mName = name;
	mCountFinished = 0;
}

PlatformLevel::~PlatformLevel() {
	unloadLevel();
}

LevelState PlatformLevel::update() {
	if(mInputManager->keyReleased(OIS::KC_P)){
		if(!mBottomPlayer->isPortal()){
			portalChar(mTopPlayer);
		}
	}
	if(mInputManager->keyReleased(OIS::KC_X)){
		if(!mTopPlayer->isPortal()){
			portalChar(mBottomPlayer);
		}
	}

	mTopPlayer->update();
	if(mTopPlayer->getHP() <= 0){
		mRunning = LS_Reset;
	}
	mBottomPlayer->update();
	if(mBottomPlayer->getHP() <= 0){
		mRunning = LS_Reset;
	}
	mTopCamMan->update();
	mBotCamMan->update();
	
	Ogre::Vector3 topPos = mTopPlayer->getDummyNode()->getPosition();
	Ogre::Vector3 bottomPos = mBottomPlayer->getDummyNode()->getPosition();
	topPos.x += 20.0f;
	topPos.y += 200.0f;
	topPos.z += 20.0f;
	bottomPos.x += 100.0f;
	bottomPos.y += 200.0f;
	bottomPos.z += 20.0f;

	mTopPointLight->setPosition(topPos);
	mBottomPointLight->setPosition(bottomPos);
	mTopEnemyMgr->update();
	mBottomEnemyMgr->update();
	
	//update all in map
	std::list<ModelEntity*>::iterator iter = mDynamicEnts.begin();
	while(iter != mDynamicEnts.end()){
		(*iter)->update();
		iter++;
	}
	return mRunning;
}

void PlatformLevel::loadLevel() {
	initScene();
	initLighting();
	initCamera();
	initModels();
	initEnemies();

	mLoaded = true;
	mRunning = LS_Keeper;
	
}

void PlatformLevel::unloadLevel() {
	mLoaded = false;
	
	mGameServices->ogreRoot()->getAutoCreatedWindow()->removeAllViewports();

	mGameServices->ogreRoot()->removeFrameListener(mTopNewtonListener);
	mGameServices->ogreRoot()->removeFrameListener(mBottomNewtonListener);
	delete mTopNewtonListener;
	delete mBottomNewtonListener;

	delete mTopCollisionWorld;
	delete mBottomCollisionWorld;

	delete mTopEnemyMgr;
	delete mBottomEnemyMgr;
	
	mTopRootNode->removeAndDestroyAllChildren();
	mBottomRootNode->removeAndDestroyAllChildren();

	mGameServices->ogreRoot()->destroySceneManager(mTopSceneMgr);
	mGameServices->ogreRoot()->destroySceneManager(mBottomSceneMgr);
}

void PlatformLevel::initScene() {
	mTopSceneMgr = mGameServices->ogreRoot()->createSceneManager(Ogre::ST_GENERIC, "TopSceneManager");
	mBottomSceneMgr = mGameServices->ogreRoot()->createSceneManager(Ogre::ST_GENERIC, "BottomSceneManager");

	mTopSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_MODULATIVE);
	mBottomSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_MODULATIVE);

	mTopRootNode = mTopSceneMgr->getRootSceneNode();
	mBottomRootNode = mBottomSceneMgr->getRootSceneNode();

	mTopLevelObjectsNode = mTopRootNode->createChildSceneNode("TopLevelObjects");
	mTopEnemiesNode = mTopRootNode->createChildSceneNode("TopEnemies");
	mTopPlayerNode = mTopRootNode->createChildSceneNode("TopPlayer");
	mTopCamNode = mTopRootNode->createChildSceneNode("TopCamera", Ogre::Vector3(0.0f,20.0f,50.0f));

	mBottomLevelObjectsNode = mBottomRootNode->createChildSceneNode("BottomLevelObjects");
	mBottomEnemiesNode = mBottomRootNode->createChildSceneNode("BottomEnemies");
	mBottomPlayerNode = mBottomRootNode->createChildSceneNode("BottomPlayer");
	mBottomCamNode = mBottomRootNode->createChildSceneNode("BottomCamera", Ogre::Vector3(0.0f,20.0f,50.0f));

	//initialize the physics world
	mTopCollisionWorld = new OgreNewt::World();
	mTopCollisionWorld->setWorldSize(Ogre::AxisAlignedBox(-2000.0f, -20.0f, -50.0f, 4000.0f, 1000.0f, 50.0f));
	mBottomCollisionWorld = new OgreNewt::World();
	mBottomCollisionWorld->setWorldSize(Ogre::AxisAlignedBox(-2000.0f, -20.0f, -50.0f, 4000.0f, 1000.0f, 50.0f));

	//add the newton listener so newton can update
	mTopNewtonListener = new OgreNewt::BasicFrameListener(mGameServices->ogreRoot()->getAutoCreatedWindow(), mTopSceneMgr, mTopCollisionWorld, mGameServices, 100);
	mBottomNewtonListener = new OgreNewt::BasicFrameListener(mGameServices->ogreRoot()->getAutoCreatedWindow(), mBottomSceneMgr, mBottomCollisionWorld, mGameServices, 100);

	mGameServices->ogreRoot()->addFrameListener(mTopNewtonListener);
	mGameServices->ogreRoot()->addFrameListener(mBottomNewtonListener);

	//setup materials for collisions
	const OgreNewt::MaterialID *matID = mTopCollisionWorld->getDefaultMaterialID();
	mTopMaterialPair = new OgreNewt::MaterialPair(mTopCollisionWorld, matID, matID);
	mTopCollisionCallback = new CollisionHandler(mGameServices);
	mTopMaterialPair->setContactCallback(mTopCollisionCallback);

	matID = mBottomCollisionWorld->getDefaultMaterialID();
	mBottomMaterialPair = new OgreNewt::MaterialPair(mBottomCollisionWorld, matID, matID);
	mBottomCollisionCallback = new CollisionHandler(mGameServices);
	mBottomMaterialPair->setContactCallback(mBottomCollisionCallback);

}

void PlatformLevel::initLighting() {
	mTopSceneMgr->setAmbientLight( Ogre::ColourValue( 0.1f, 0.1f, 0.1f ) );	//ambient white light
	mBottomSceneMgr->setAmbientLight( Ogre::ColourValue( 0.1f, 0.1f, 0.1f ) );	//ambient white light

	mTopPointLight = mTopSceneMgr->createLight("TopPointLight");
	mTopPointLight->setPosition(0.0f, 200.0f, 0.0f);
	mTopPointLight->setType(Ogre::Light::LT_POINT);
	mTopPointLight->setCastShadows(true);
	mTopPointLight->setAttenuation(500.0f, 0.2f, 0.0f, 0.0f);

	mBottomPointLight = mBottomSceneMgr->createLight("BottomPointLight");
	mBottomPointLight->setPosition(0.0f, 200.0f, 0.0f);
	mBottomPointLight->setType(Ogre::Light::LT_POINT);
	mBottomPointLight->setCastShadows(true);
	mBottomPointLight->setAttenuation(500.0f, 0.2f, 0.0f, 0.0f);
}

void PlatformLevel::initCamera() {
	mTopCam = mTopSceneMgr->createCamera("TopCamera");// create the camera
	mTopCam->setPosition(Ogre::Vector3(0.0f, 75.0f, 150.0f));	// set its position, direction  
	mTopCam->setDirection(Ogre::Vector3(-0.0f, -0.32f, -1.0f));
	mTopCam->setNearClipDistance(5);	//makes everything closer than this dist invisible

	mTopCamNode->attachObject(mTopCam);
	
	//make second camera
	mBottomCam = mBottomSceneMgr->createCamera("BottomCamera");
	mBottomCam->setPosition(Ogre::Vector3(0.0f, 75.0f, 150.0f));	//point at origin, its where the model is
	mBottomCam->lookAt(Ogre::Vector3(0.0f,0.0f,0.0f));
	mBottomCam->setNearClipDistance(5);	//makes everything closer than this dist invisible

	mBottomCamNode->attachObject(mBottomCam);

	Ogre::RenderWindow *win = mGameServices->ogreRoot()->getAutoCreatedWindow();

	win->removeAllViewports();	//need to remove all previously created viewports

	Ogre::Viewport *viewportTop = win->addViewport(mTopCam, 0, 0, 0, 1, 0.5);
	viewportTop->setBackgroundColour(Ogre::ColourValue(1,0,0));
	mTopCam->setAspectRatio(Ogre::Real(viewportTop->getActualWidth()) / Ogre::Real(viewportTop->getActualHeight()));

	Ogre::Viewport *viewportBottom = win->addViewport(mBottomCam, 1, 0, 0.5, 1, 0.5);	// .5 to height, and .5 to 'top'
	viewportBottom->setBackgroundColour(Ogre::ColourValue(0,0,1));
	mBottomCam->setAspectRatio(Ogre::Real(viewportBottom->getActualWidth()) / Ogre::Real(viewportBottom->getActualHeight()));
}

void PlatformLevel::initModels(){
	//players
	mTopPlayer = new PlayerEntity(mGameServices, mTopCollisionWorld, mTopPlayerNode, mTopCamNode, 
		Ogre::Vector3(-1300.0f, 150.0f, 0.0f), Ogre::Vector3(1.0f), "Player1", "../assets/munny.mesh");
	mBottomPlayer = new PlayerEntity(mGameServices, mBottomCollisionWorld, mBottomPlayerNode, mBottomCamNode, 
		Ogre::Vector3(-1300.0f, 150.0f, 0.0f), Ogre::Vector3(1.0f), "Player2", "../assets/munny2.mesh");
	mTopCamMan = new PlatformCamera(mTopCamNode,mTopPlayer);
	mBotCamMan = new PlatformCamera(mBottomCamNode,mBottomPlayer);
	
	std::string levelFile;
	//mGameServices->gameConfig()->getData("firstLevel", levelFile);
	LevelParser feces;
	feces.parseLevel(mTopFilePath);

	std::vector<EntityProperties> modelList = feces.getVect();

	if(mNextLevel == LS_One){
		MovingBlock *mbptr;
		OneTimeTrigger *ottptr;
		HazardTrigger *htptr;
		PushBlockRight *pbrptr;
		ModelEntity *entptr;
		PressureSensor *psptr;
		for(unsigned int i=0;i<modelList.size();i++){
			if(modelList[i].Name == "Node3"){ // Box1
				pbrptr = new PushBlockRight(mGameServices, mTopCollisionWorld, mTopLevelObjectsNode, 
					modelList[i].Position, modelList[i].Scale,
					modelList[i].Name+"top","../assets/" + modelList[i].Meshfile, "push_box1top","push_box1top", 
					0, Ogre::Vector3(modelList[i].Position.x+250,modelList[i].Position.y, modelList[i].Position.z), 1);
				pbrptr->setMaterial(modelList[i].Materialfile);
				entptr = dynamic_cast<ModelEntity*>(pbrptr);
				addEntity( entptr, true );
			}
			else if(modelList[i].Name == "Node4"){ //Box2
				pbrptr = new PushBlockRight(mGameServices, mTopCollisionWorld, mTopLevelObjectsNode, 
					modelList[i].Position, modelList[i].Scale,
					modelList[i].Name+"top", "../assets/" + modelList[i].Meshfile, "push_box2top","push_box2bot", 
					0, Ogre::Vector3(modelList[i].Position.x+200,modelList[i].Position.y, modelList[i].Position.z) ,1);
				pbrptr->setMaterial(modelList[i].Materialfile);
				entptr = dynamic_cast<ModelEntity*>(pbrptr);
				addEntity( entptr, true );
			}
			else if(modelList[i].Name == "Node9"){ //water
				htptr = new HazardTrigger(mGameServices, mTopCollisionWorld, mTopLevelObjectsNode,
					modelList[i].Position, modelList[i].Scale, 
					"TopWater1", "../assets/" + modelList[i].Meshfile, "alpha_WaterHazard_");
				htptr->setMaterial(modelList[i].Materialfile);
				htptr->setKill(modelList[i].Kill);
				//Ice
				mbptr = new MovingBlock(mGameServices, mTopCollisionWorld, mTopLevelObjectsNode, 
					Ogre::Vector3(modelList[i].Position.x,  modelList[i].Position.y  -100, modelList[i].Position.z), modelList[i].Scale, 
					"TopIce", "../assets/high_def_cube.mesh", 
					"alpha_OneTimeTrigger_Canister_bot", 
					Ogre::Vector3(modelList[i].Position.x,  modelList[i].Position.y + 10, modelList[i].Position.z));
				//ottptr->setVis(false);
			}
			else if(modelList[i].Name == "Node57"){
				psptr = new PressureSensor(mGameServices, mTopCollisionWorld, mTopLevelObjectsNode,
					modelList[i].Position, modelList[i].Scale,modelList[i].Name, "../assets/" + modelList[i].Meshfile,
					"Trigger_Raise58_top",1);
				psptr->setMaterial(modelList[i].Materialfile);
			}
			else if(modelList[i].Name == "Node58"){	// Rising platform lets player hit drawbridge
				RisingPlatform *rpptr = new RisingPlatform(mGameServices, mTopCollisionWorld, mTopLevelObjectsNode,
					modelList[i].Position, modelList[i].Scale,modelList[i].Name+"_Top","../assets/" + modelList[i].Meshfile,
					"Trigger_Raise58_top");
				rpptr->setMaterial(modelList[i].Materialfile);
				rpptr->setOrientation(modelList[i].Orientation);
				entptr = dynamic_cast<ModelEntity*>(rpptr);
				addEntity( entptr, true );
			}
			else if(modelList[i].Name == "Node16"){ //break-down wall
				Drawbridge* dbptr = new Drawbridge(mGameServices, mTopCollisionWorld, mTopLevelObjectsNode, 
					modelList[i].Position, modelList[i].Scale, 
					modelList[i].Name+"Bot", "../assets/" + modelList[i].Meshfile, 
					"Trigger_Drawbridge");
				dbptr->setMaterial(modelList[i].Materialfile);
				dbptr->getDummyNode()->setOrientation(modelList[i].Orientation);
				dbptr->updateNewtonBody();	
				dbptr->setOrientation(modelList[i].Orientation);
				ottptr = new OneTimeTrigger(mGameServices, mTopCollisionWorld, mTopLevelObjectsNode, 
					Ogre::Vector3(modelList[i].Position.x-5.0f, modelList[i].Position.y-23,modelList[i].Position.z), 
					Ogre::Vector3(10.0f, 0.1f, 70.0f), 
					"TopTriggerDrawbridge", "../assets/high_def_cube.mesh","Trigger_Drawbridge", 1);
				ottptr->setVis(false);
			}
			else if(modelList[i].Name == "Node36"){ //switch unblocks platform41 so it can continue
				ottptr = new OneTimeTrigger(mGameServices, mTopCollisionWorld, mTopLevelObjectsNode, 
					modelList[i].Position, modelList[i].Scale, modelList[i].Name, "../assets/" + modelList[i].Meshfile,
					"Trigger_Sink44", 1);
			}
			else if(modelList[i].Name == "Node37"){// these switches only appear in top
				psptr = new PressureSensor(mGameServices, mTopCollisionWorld, mTopLevelObjectsNode, 
					modelList[i].Position, modelList[i].Scale, modelList[i].Name, "../assets/" + modelList[i].Meshfile,
					"Trigger_StartPlatform41_A", 1);
			}
			else if(modelList[i].Name == "Node38"){
				psptr = new PressureSensor(mGameServices, mTopCollisionWorld, mTopLevelObjectsNode, 
					modelList[i].Position, modelList[i].Scale, modelList[i].Name, "../assets/" + modelList[i].Meshfile,
					"Trigger_StartPlatform41_B", 1);
			}
			else if(modelList[i].Name == "Node41"){	// moving platform with multiple stages
				Platform41* p41ptr = new Platform41(mGameServices, mTopCollisionWorld, mTopLevelObjectsNode,
					modelList[i].Position, modelList[i].Scale, "Platform41_top" , "../assets/" + modelList[i].Meshfile,
					"Trigger_StartPlatform41");
				p41ptr->setMaterial(modelList[i].Materialfile);
				p41ptr->getDummyNode()->setOrientation(modelList[i].Orientation);
				p41ptr->updateNewtonBody();	
				p41ptr->setOrientation(modelList[i].Orientation);
				entptr = dynamic_cast<ModelEntity*>(p41ptr);
				addEntity( entptr, true );
			}
			else if(modelList[i].Name == "Node43"){} //only exists in bottom
			else if(modelList[i].Name == "Node44"){	//moves down to unblock 41
				mbptr = new MovingBlock(mGameServices, mTopCollisionWorld, mTopLevelObjectsNode,
					modelList[i].Position, modelList[i].Scale, modelList[i].Name+"_Bot", "../assets/" + modelList[i].Meshfile,
					"Trigger_Sink44", Ogre::Vector3(modelList[i].Position.x,  modelList[i].Position.y - 100, modelList[i].Position.z));
			}
			else if(modelList[i].Name == "Node53"){} // canister only exists in bottom
			else if(modelList[i].Name == "Node39"){ //water
				htptr = new HazardTrigger(mGameServices, mTopCollisionWorld, mTopLevelObjectsNode,
					modelList[i].Position, modelList[i].Scale, 
					"BotWater1", "../assets/" + modelList[i].Meshfile, "alpha_WaterHazard_");
				htptr->setMaterial(modelList[i].Materialfile);
				htptr->setKill(modelList[i].Kill);
				//ice
				mbptr = new MovingBlock(mGameServices, mTopCollisionWorld, mTopLevelObjectsNode, 
					Ogre::Vector3(modelList[i].Position.x,  modelList[i].Position.y  -100, modelList[i].Position.z), modelList[i].Scale, 
					"BotIce", "../assets/high_def_cube.mesh", 
					"Trigger_Canister_43", 
					Ogre::Vector3(modelList[i].Position.x,  modelList[i].Position.y + 10, modelList[i].Position.z));
			}
			else{
				mModel = new RigidModelEntity(mGameServices, mTopCollisionWorld, mTopLevelObjectsNode,
					modelList[i].Position, modelList[i].Scale, modelList[i].Name, "../assets/" + modelList[i].Meshfile);
				mModel->setMaterial(modelList[i].Materialfile);
				mModel->getDummyNode()->setOrientation(modelList[i].Orientation);
				mModel->setKill(modelList[i].Kill);
				mModel->updateNewtonBody();
			}	
		}
		// BOTTOM LEVEL OBJECTS
		LevelParser poop;
		poop.parseLevel(mBottomFilePath);

		modelList = poop.getVect();
		for(unsigned int i=0;i<modelList.size();i++){
			if(modelList[i].Name == "Node3"){ // Box1
				pbrptr = new PushBlockRight(mGameServices, mBottomCollisionWorld, mBottomLevelObjectsNode, 
					modelList[i].Position, modelList[i].Scale,
					 modelList[i].Name, "../assets/" + modelList[i].Meshfile, "push_box1bot", "push_box1top", 0,
					 Ogre::Vector3(modelList[i].Position.x+250,modelList[i].Position.y, modelList[i].Position.z), 1);
				pbrptr->setMaterial(modelList[i].Materialfile);
				entptr = dynamic_cast<ModelEntity*>(pbrptr);
				addEntity( entptr, true );
			}
			else if(modelList[i].Name == "Node4"){ //Box2
				pbrptr = new PushBlockRight(mGameServices, mBottomCollisionWorld, mBottomLevelObjectsNode, 
					modelList[i].Position, modelList[i].Scale,
					modelList[i].Name, "../assets/" + modelList[i].Meshfile, "push_box2bot", "push_box2bot", 0,
					Ogre::Vector3(modelList[i].Position.x+200,modelList[i].Position.y, modelList[i].Position.z), 1);
				pbrptr->setMaterial(modelList[i].Materialfile);
				entptr = dynamic_cast<ModelEntity*>(pbrptr);
				addEntity( entptr, true );
			}
			else if(modelList[i].Name == "Node9"){ //water
				htptr = new HazardTrigger(mGameServices, mBottomCollisionWorld, mBottomLevelObjectsNode,
					modelList[i].Position, modelList[i].Scale, 
					"BotWater1", "../assets/" + modelList[i].Meshfile, "alpha_WaterHazard_");
				htptr->setMaterial(modelList[i].Materialfile);
				htptr->setKill(modelList[i].Kill);
				// canister
				mbptr = new MovingBlock(mGameServices, mBottomCollisionWorld, mBottomLevelObjectsNode, 
					Ogre::Vector3(-750.0f,  35.0f, 0.0f), Ogre::Vector3(10.0f, 10.0f, 10.0f), 
					"BotCanister", "../assets/co2_tank.mesh", 
					"alpha_OneTimeTrigger_Canister_bot", Ogre::Vector3(-750.0f,  -50.0f, 0.0f));
				ottptr = new OneTimeTrigger(mGameServices, mBottomCollisionWorld, mBottomLevelObjectsNode, 
					Ogre::Vector3(-762.0f,  35.0f, 5.0f), Ogre::Vector3(5.0f, 5.0f, 15.0f), 
					"BotTriggerCanister", "../assets/high_def_cube.mesh","alpha_OneTimeTrigger_Canister_bot", 1);
				//Ice
				mbptr = new MovingBlock(mGameServices, mBottomCollisionWorld, mBottomLevelObjectsNode, 
					Ogre::Vector3(modelList[i].Position.x,  modelList[i].Position.y  -100, modelList[i].Position.z), modelList[i].Scale, 
					"BotIce", "../assets/high_def_cube.mesh", 
					"alpha_OneTimeTrigger_Canister_bot", 
					Ogre::Vector3(modelList[i].Position.x,  modelList[i].Position.y + 10, modelList[i].Position.z));
				ottptr->setVis(false);
			}
			else if(modelList[i].Name == "Node57"){
				psptr = new PressureSensor(mGameServices, mBottomCollisionWorld, mBottomLevelObjectsNode,
					modelList[i].Position, modelList[i].Scale,modelList[i].Name, "../assets/" + modelList[i].Meshfile,
					"Trigger_Raise58_bot",1);
				psptr->setMaterial(modelList[i].Materialfile);
			}
			else if(modelList[i].Name == "Node58"){	// Rising platform lets player hit drawbridge
				RisingPlatform *rpptr = new RisingPlatform(mGameServices, mBottomCollisionWorld, mBottomLevelObjectsNode,
					modelList[i].Position, modelList[i].Scale,modelList[i].Name+"_Bot","../assets/" + modelList[i].Meshfile,
					"Trigger_Raise58_bot");
				rpptr->setMaterial(modelList[i].Materialfile);
				rpptr->setOrientation(modelList[i].Orientation);
				entptr = dynamic_cast<ModelEntity*>(rpptr);
				addEntity( entptr, true );
			}
			else if(modelList[i].Name == "Node16"){ //break-down wall
				Drawbridge* dbptr = new Drawbridge(mGameServices, mBottomCollisionWorld, mBottomLevelObjectsNode, 
					modelList[i].Position, modelList[i].Scale, 
					modelList[i].Name+"Bot", "../assets/" + modelList[i].Meshfile, 
					"Trigger_Drawbridge");
				dbptr->setMaterial(modelList[i].Materialfile);
				dbptr->getDummyNode()->setOrientation(modelList[i].Orientation);
				dbptr->updateNewtonBody();	
				dbptr->setOrientation(modelList[i].Orientation);
				ottptr = new OneTimeTrigger(mGameServices, mBottomCollisionWorld, mBottomLevelObjectsNode, 
					Ogre::Vector3(modelList[i].Position.x-5.0f, modelList[i].Position.y-23,modelList[i].Position.z), 
					Ogre::Vector3(10.0f, 0.1f, 70.0f), 
					"BotTriggerDrawbridge", "../assets/high_def_cube.mesh","Trigger_Drawbridge", 1);
				ottptr->setVis(false);
			}
			else if(modelList[i].Name == "Node36"){}
			else if(modelList[i].Name == "Node37"){}// these switches only appear in top
			else if(modelList[i].Name == "Node38"){}
			else if(modelList[i].Name == "Node41"){	// moving platform with multiple stages
				Platform41* p41ptr = new Platform41(mGameServices, mBottomCollisionWorld, mBottomLevelObjectsNode,
					modelList[i].Position, modelList[i].Scale, "Platform41_bot" , "../assets/" + modelList[i].Meshfile,
					"Trigger_StartPlatform41");
				p41ptr->setMaterial(modelList[i].Materialfile);
				p41ptr->getDummyNode()->setOrientation(modelList[i].Orientation);
				p41ptr->updateNewtonBody();	
				p41ptr->setOrientation(modelList[i].Orientation);
				entptr = dynamic_cast<ModelEntity*>(p41ptr);
				addEntity( entptr, true );
			}
			else if(modelList[i].Name == "Node43"){	//unique to bot world, switch to move platform41 to phase 2
				ottptr = new OneTimeTrigger(mGameServices, mBottomCollisionWorld, mBottomLevelObjectsNode, 
					modelList[i].Position, modelList[i].Scale ,
					modelList[i].Name, "../assets/" + modelList[i].Meshfile,"Trigger_Platform41_Phase2", 1);
			} 
			else if(modelList[i].Name == "Node44"){	//moves down to unblock 41
				mbptr = new MovingBlock(mGameServices, mBottomCollisionWorld, mBottomLevelObjectsNode,
					modelList[i].Position, modelList[i].Scale, modelList[i].Name+"_Bot",  "../assets/" + modelList[i].Meshfile,
					"Trigger_Sink44", Ogre::Vector3(modelList[i].Position.x,  modelList[i].Position.y - 100, modelList[i].Position.z));
			}
			else if(modelList[i].Name == "Node53"){ //canister
				mbptr = new MovingBlock(mGameServices, mBottomCollisionWorld, mBottomLevelObjectsNode, 
					modelList[i].Position, modelList[i].Scale, modelList[i].Name,
					"../assets/" + modelList[i].Meshfile, "Trigger_Canister_43",
					 Ogre::Vector3(modelList[i].Position.x,modelList[i].Position.y-100,modelList[i].Position.z));
				ottptr = new OneTimeTrigger(mGameServices, mBottomCollisionWorld, mBottomLevelObjectsNode, 
					Ogre::Vector3(modelList[i].Position.x-5.0f, modelList[i].Position.y,modelList[i].Position.z), 
					Ogre::Vector3(5.0f, 5.0f, 5.0f), 
					"BotTriggerCanister53", "../assets/high_def_cube.mesh","Trigger_Canister_43", 1);
				ottptr->setVis(false);
			}
			else if(modelList[i].Name == "Node39"){ //water
				htptr = new HazardTrigger(mGameServices, mBottomCollisionWorld, mBottomLevelObjectsNode,
					modelList[i].Position, modelList[i].Scale, 
					"BotWater1", "../assets/" + modelList[i].Meshfile, "alpha_WaterHazard_");
				htptr->setMaterial(modelList[i].Materialfile);
				htptr->setKill(modelList[i].Kill);
				//ice
				mbptr = new MovingBlock(mGameServices, mBottomCollisionWorld, mBottomLevelObjectsNode, 
					Ogre::Vector3(modelList[i].Position.x,  modelList[i].Position.y  -100, modelList[i].Position.z), modelList[i].Scale, 
					"BotIce", "../assets/high_def_cube.mesh", 
					"Trigger_Canister_43", 
					Ogre::Vector3(modelList[i].Position.x,  modelList[i].Position.y + 10, modelList[i].Position.z));
			}
			else{
				mModel = new RigidModelEntity(mGameServices, mBottomCollisionWorld, mBottomLevelObjectsNode,
					modelList[i].Position, modelList[i].Scale, modelList[i].Name, "../assets/" + modelList[i].Meshfile);
				mModel->setMaterial(modelList[i].Materialfile);
				mModel->getDummyNode()->setOrientation(modelList[i].Orientation);
				mModel->setKill(modelList[i].Kill);
				mModel->updateNewtonBody();
			}
		}
		/*
		ottptr = new OneTimeTrigger(mGameServices, mTopCollisionWorld, mTopLevelObjectsNode, 
									Ogre::Vector3(3180.0f, 115.0f, 0.0f), Ogre::Vector3(5.0f, 100.0f, 100.0f), 
									"TopEndOfLevel", "../assets/high_def_cube.mesh","alpha_EndOfLevel", 1); 
		ottptr->setVis(false);
		entptr = dynamic_cast<ModelEntity*>(ottptr);
		addEntity( entptr, false );
		ottptr = new OneTimeTrigger(mGameServices, mBottomCollisionWorld, mBottomLevelObjectsNode, 
									Ogre::Vector3(3180.0f, 115.0f, 0.0f), Ogre::Vector3(5.0f, 100.0f, 100.0f), 
									"BotEndOfLevel", "../assets/high_def_cube.mesh","alpha_EndOfLevel", 2); 
		ottptr->setVis(false);
		entptr = dynamic_cast<ModelEntity*>(ottptr);
		addEntity( entptr, false );*/
	}
	if(mNextLevel == LS_Two){
		for(unsigned int i=0;i<modelList.size();i++){
			mModel = new RigidModelEntity(mGameServices, mTopCollisionWorld, mTopLevelObjectsNode,
				modelList[i].Position, modelList[i].Scale, modelList[i].Name, "../assets/" + modelList[i].Meshfile);
			mModel->setMaterial(modelList[i].Materialfile);
			mModel->getDummyNode()->setOrientation(modelList[i].Orientation);
			mModel->setKill(modelList[i].Kill);
			mModel->updateNewtonBody();
		}
		LevelParser poop;
		poop.parseLevel(mBottomFilePath);

		modelList = poop.getVect();
		for(unsigned int i=0;i<modelList.size();i++){
			mModel = new RigidModelEntity(mGameServices, mBottomCollisionWorld, mBottomLevelObjectsNode,
				modelList[i].Position, modelList[i].Scale, modelList[i].Name, "../assets/" + modelList[i].Meshfile);
			mModel->setMaterial(modelList[i].Materialfile);
			mModel->getDummyNode()->setOrientation(modelList[i].Orientation);
			mModel->setKill(modelList[i].Kill);
			mModel->updateNewtonBody();
		}
	}


		//register end level event
	//mGameServices->events()->addListener("alpha_EndOfLevel", this, &PlatformLevel::endLevel);
}

void PlatformLevel::initEnemies() {
	float swarmSizeMin, swarmSizeMax, swarmRateMin, swarmRateMax, enemyRateMin, enemyRateMax;
	mGameServices->gameConfig()->getData("enemyWaveSpawnRateMin", swarmRateMin);
	mGameServices->gameConfig()->getData("enemyWaveSpawnRateMax", swarmRateMax);
	mGameServices->gameConfig()->getData("enemySpawnRateMin", enemyRateMin);
	mGameServices->gameConfig()->getData("enemySpawnRateMax", enemyRateMax);
	mGameServices->gameConfig()->getData("enemyWaveSizeMin", swarmSizeMin);
	mGameServices->gameConfig()->getData("enemyWaveSizeMax", swarmSizeMax);

	mTopEnemyMgr = new EnemyManager(mGameServices, mTopCollisionWorld, mTopEnemiesNode, "../assets/munny.mesh", mTopCam);
	mTopEnemyMgr->setSwarmSize(swarmSizeMin, swarmSizeMax);
	mTopEnemyMgr->setSwarmSpawnRate(swarmRateMin, swarmRateMax);
	mTopEnemyMgr->setEnemySpawnRate(enemyRateMin, enemyRateMax);
	mTopEnemyMgr->setTarget(mTopPlayer->getDummyNode());

	mTopEnemyMgr->spawnEnemies(true);

	mBottomEnemyMgr = new EnemyManager(mGameServices, mBottomCollisionWorld, mBottomEnemiesNode, "../assets/munny.mesh", mBottomCam);
	mBottomEnemyMgr->setSwarmSize(swarmSizeMin, swarmSizeMax);
	mBottomEnemyMgr->setSwarmSpawnRate(swarmRateMin, swarmRateMax);
	mBottomEnemyMgr->setEnemySpawnRate(enemyRateMin, enemyRateMax);
	mBottomEnemyMgr->setTarget(mBottomPlayer->getDummyNode());

	mBottomEnemyMgr->spawnEnemies(true);
}

void PlatformLevel::addEntity(ModelEntity *entity, bool needUpdate){
	mStaticEnts[entity->name()] = entity;	// access by name
	if( needUpdate ){	// also add copies of dynamic ents into the dynamic list
		mDynamicEnts.push_back( entity );
	}
}
int PlatformLevel::removeEntity( ModelEntity *entity, bool needUpdate){
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
void PlatformLevel::endLevel(GameEvent *evnt){
	mCountFinished++;
	if(mCountFinished >= 2){
		mGameServices->debugOut("TODO: end level!\n");
		mRunning = LS_Arena;
	}
}
void PlatformLevel::portalChar(PlayerEntity* player){
	if(player->name() == "Player1"){
		if(player->isPortal()){
			player->portal(mTopCollisionWorld, mTopPlayerNode, player->getOldPos());
			mTopEnemyMgr->setTarget(mTopPlayer->getDummyNode());
		}
		else{
			player->portal(mBottomCollisionWorld, mBottomPlayerNode, player->position());
			mTopEnemyMgr->setTarget(NULL);
		}
	}
	else{
		if(player->isPortal()){
			player->portal(mBottomCollisionWorld, mBottomPlayerNode, player->getOldPos());
			mBottomEnemyMgr->setTarget(mBottomPlayer->getDummyNode());
		}
		else{
			player->portal(mTopCollisionWorld, mTopPlayerNode, player->position());
			mBottomEnemyMgr->setTarget(NULL);
		}
	}


	//Detach player node from root node.
	//Reattach to opposite root node.
	//Change collision world.
	//Move character to other character's position.

}
void PlatformLevel::getPlayers(PlayerEntity* p1, PlayerEntity* p2){
	p1 = mTopPlayer;
	p2 = mBottomPlayer;
}


