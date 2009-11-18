#include "Game.h"
#include "Munny.h"
#include "PlatformLevel.h"
#include "ArenaLevel.h"
#include "IntroLevel.h"
#include "GameServices.h"
#include "TextRenderer.h"
#include "InputManager.h"

Game::Game(Ogre::Root *ogreRoot, GameServices *gameServices) {
	mOgreRoot = ogreRoot;
	mGameServices = gameServices;
	mGameClock = new Ogre::Timer();
	mFrameTimer = new Ogre::Timer();

	init();
	p1_HP = 0;
	p2_HP = 0;

	mTargetFPS = 35.0f;
}

Game::~Game() {
	delete mLevel;
	delete mGameClock;
	delete mFrameTimer;
}

void Game::init() {
	
	//all initializing code for managers or anything
	//else that is part of the game should go here.
	//mSceneMgr = mOgreRoot->getSceneManager("Default SceneManager");	//pointer to SceneManager
	//mSceneMgr->setShadowTechnique( Ogre::SHADOWTYPE_STENCIL_ADDITIVE );

	mWindow = mOgreRoot->getAutoCreatedWindow();	//pointer to RenderWindow for convenience
	std::string topLevelOne = "../../assets/examp/choreo.ogscene";
	std::string bottomLevelOne = "../../assets/examp/choreo_urban.ogscene";
	std::string topLevelTwo = "../../assets/examp/choreo2.ogscene";
	std::string bottomLevelTwo = "../../assets/examp/choroe2_urban.ogscene";
	int choice;

	mGameServices->gameConfig()->getData("firstLevel", choice);
	choice = 3;

	if(choice == 0){
		mLevel = new IntroLevel(mGameServices);
	}
	if(choice == 1){
		mLevel = new PlatformLevel(mGameServices, "../../assets/examp/choreo.ogscene", "../../assets/examp/choreo_urban.ogscene", "Platform Level");
		mLevel->setLevel(LS_One);
	}
	if(choice == 2){
		mLevel = new PlatformLevel(mGameServices, "../../assets/examp/choreo2.ogscene", "../../assets/examp/choroe2_urban.ogscene", "Platform Level");
		mLevel->setLevel(LS_Two);
	}
	if(choice == 3){
		mLevel = new ArenaLevel(mGameServices, "", "Arena Level");
	}
	
	//mLevel = new PlatformLevel(mGameServices, "../../assets/examp/choreo.ogscene", "../../assets/examp/choreo_urban.ogscene", "Platform Level");
	mLevel->loadLevel();
	mGameClock->reset();
	
	//Attempt at text overlay, uncommenting this causes gray thing to show up onscreen.
	//mTextRenderer = new TextRenderer();
	//mTextRenderer->getSingleton().addTextBox("2", "TIME", 0, 0, 10000, 10000, Ogre::ColourValue::Green);

}

void Game::startUpdate() {
	mFrameTimer->reset();
	/* prints the fps in the debug output
	std::ostringstream stm;
	stm << mOgreRoot->getAutoCreatedWindow()->getLastFPS();
	mGameServices->debugOut(stm.str().c_str()); */
	
	if(mGameServices->input()->keyDown(OIS::KC_Y)) {
		delete mLevel;
		mLevel = new PlatformLevel(mGameServices, "../../assets/examp/choreo.ogscene", "../../assets/examp/choreo_urban.ogscene", "Platform Level");
		//mLevel = new ArenaLevel(mGameServices, "", "Arena Level");
		//mLevel = new IntroLevel(mGameServices);
		mLevel->setLevel(LS_One);
		mLevel->loadLevel();
	}

	if(mLevel){
		LevelState b = mLevel->update();
		if(b == LS_Arena){
			mGameServices->debugOut("Deleting Level!\n");
			//save player state!
			//PlayerEntity *p1;
			//PlayerEntity *p2;
			//mLevel->getPlayers(p1, p2);
			//p1_HP = p1->getHP();
			//p2_HP = p2->getHP();
			//mLevel->unloadLevel();
			//mGameServices->setActive(false);
			//mLevel = NULL;
			delete mLevel;
			mLevel = new ArenaLevel(mGameServices, "", "Arena Level");
			mLevel->loadLevel();
			mLevel->setLevel(LS_Two);
		}
		if(b == LS_Reset){
			mGameServices->debugOut("Reseting Level!\n");
			std::string resetLevel; 
			std::string botResetLevel;
			LevelState tempLevel;
			if(mLevel->getLevel() == LS_One){
				resetLevel = "../../assets/examp/choreo.ogscene";
				botResetLevel = "../../assets/examp/choreo_urban.ogscene";
				tempLevel = LS_One;
			}
			else{
				resetLevel = "../../assets/examp/choreo2.ogscene";
				botResetLevel = "../../assets/examp/choroe2_urban.ogscene";
				tempLevel = LS_Two;
			}
			mLevel->unloadLevel();
			mLevel = new PlatformLevel(mGameServices, resetLevel, botResetLevel, "Platform Level");
			mLevel->setLevel(tempLevel);
			mLevel->loadLevel();
		}
		if(b == LS_One){
			mGameServices->debugOut("Loading Level 1!\n");
			mLevel->unloadLevel();
			mLevel = new PlatformLevel(mGameServices, "../../assets/examp/choreo.ogscene", "../../assets/examp/choreo_urban.ogscene", "Platform Level");
			mLevel->setLevel(LS_One);
			mLevel->loadLevel();
		}
		if(b == LS_Two){
			mGameServices->debugOut("Loading Level 2!\n");
			mLevel->unloadLevel();
			mLevel = new PlatformLevel(mGameServices, "../../assets/examp/choreo2.ogscene", "../../assets/examp/choroe2_urban.ogscene", "Platform Level");
			mLevel->setLevel(LS_Two);
			mLevel->loadLevel();
		}
	}
}

void Game::endUpdate() {
	float deltaTime = mFrameTimer->getMilliseconds();
	float frameTime = 1000.0f / mTargetFPS;

	if(deltaTime < frameTime) {
		Sleep(frameTime - deltaTime);
	}
}
