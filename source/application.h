// Application
// Main entry and initialization for the ogre application. The game is
// started in the funciton createFrameListener(). This function adds a
// gameListener. GameListener is responsible for initializing and updating
// the game. The GUI system is also initialized here and its listener will
// be added here.

#ifndef APPLICATION_H
#define APPLICATION_H

#include <Ogre.h>
#include <OIS/OIS.h>
#include <CEGUI/CEGUI.h>
#include <OgreCEGUIRenderer.h>
using namespace Ogre;

class GameListener;

class Application
{
public:
	Application();
	~Application();

	void go();

private:
	Root *mRoot;
	CEGUI::OgreCEGUIRenderer *mRenderer;
	CEGUI::System *mSystem;
	GameListener *mGameListener;

	void createRoot();
	void defineResources();
	void setupRenderSystem();
	void createRenderWindow();
	void initializeResourceGroups();
	void setupScene();
	void setupCEGUI();
	void createFrameListener();
	void startRenderLoop();
};

#endif