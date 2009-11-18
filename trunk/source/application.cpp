#include "Application.h"
#include "GameListener.h"

Application::Application() {
	mRoot = NULL;
	mRenderer = NULL;
	mSystem = NULL;
	mGameListener = NULL;
}

Application::~Application() {
	mRoot->removeFrameListener(mGameListener);
	delete mGameListener;
	delete mRenderer;
	delete mSystem;
	//delete mRoot;
}

void Application::go()
{
	createRoot();
	defineResources();
	setupRenderSystem();
	createRenderWindow();
	initializeResourceGroups();
	setupScene();
	setupCEGUI();
	createFrameListener();
	startRenderLoop();
}

void Application::createRoot()
{
	mRoot = new Root();
}

void Application::defineResources()
{
	String secName, typeName, archName;
	ConfigFile cf;
	cf.load("resources.cfg");

	ConfigFile::SectionIterator seci = cf.getSectionIterator();
	while (seci.hasMoreElements())
	{
		secName = seci.peekNextKey();
		ConfigFile::SettingsMultiMap *settings = seci.getNext();
		ConfigFile::SettingsMultiMap::iterator i;
		for (i = settings->begin(); i != settings->end(); ++i)
		{
			typeName = i->first;
			archName = i->second;
			ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
		}
	}
}

void Application::setupRenderSystem()
{
	if (!mRoot->restoreConfig() && !mRoot->showConfigDialog()) {
		throw Exception(52, "User canceled the config dialog!", "Application::setupRenderSystem()");

	}
}

void Application::createRenderWindow()
{
	mRoot->initialise(true, "Choreo Power");
	mRoot->setFrameSmoothingPeriod(4);
}

void Application::initializeResourceGroups()
{
	TextureManager::getSingleton().setDefaultNumMipmaps(5);
	ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

void Application::setupScene()
{
	SceneManager *mgr = mRoot->createSceneManager(ST_GENERIC, "FECES");
	//Camera *cam = mgr->createCamera("Camera");
	//Viewport *vp = mRoot->getAutoCreatedWindow()->addViewport(cam);
}


void Application::setupCEGUI()
{
	//SceneManager *mgr = mRoot->getSceneManager("Default SceneManager");
	RenderWindow *win = mRoot->getAutoCreatedWindow();

	// CEGUI setup
	//mRenderer = new CEGUI::OgreCEGUIRenderer(win, Ogre::RENDER_QUEUE_OVERLAY, false, 3000, mgr);
	//mSystem = new CEGUI::System(mRenderer);

	// Other CEGUI setup here.
	/*CEGUI::WindowManager *wind = CEGUI::WindowManager::getSingletonPtr();
	CEGUI::Window *sheet = wind->createWindow("DefaultGUISheet", "CEGUIDemo/Sheet");

	CEGUI::Window *quit = wind->createWindow("TaharezLook/Button", "CEGUIDemo/QuitButton");
	quit->setText("Quit");
	quit->setSize(CEGUI::UVector2(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));

	sheet->addChildWindow(quit);
	mSystem->setGUISheet(sheet);*/
}

void Application::createFrameListener()
{
	mGameListener = new GameListener(mRoot);
	mRoot->addFrameListener(mGameListener);
}

void Application::startRenderLoop()
{
	mRoot->startRendering();
}