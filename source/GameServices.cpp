#include "GameServices.h"
#include "InputManager.h"
#include "EventManager.h"
#include "Game.h"
#include <windows.h>

#ifndef NDEBUG
  #define DPRINT(text, ...)             dprintf(__FILE__, __LINE__, text, __VA_ARGS__)
  void dprintf(const char* szFile, long lLine, const char* fmt, ...);
#else // #ifdef NDEBUG
  #define DPRINT(text, ...)             ((void)0)
#endif

GameServices::GameServices(Ogre::Root *ogreRoot) {
	mOgreRoot = ogreRoot;
	mActive = true;
	mInputManager = new InputManager(ogreRoot->getAutoCreatedWindow());
	mEventManager = new EventManager();
	mGameConfig = new GameConfig();
	mGameConfig->loadConfigFile("game.cfg");
}

GameServices::~GameServices() {
	delete mInputManager;
}

void GameServices::update() {
	if(mActive)
		mInputManager->update();
}
void GameServices::setActive(bool b){
	mActive = b;
}
void GameServices::debugOut(const char* fmt, ...){
	static const unsigned int OutputMaxLength = 1024;
	char output[OutputMaxLength];
	char* pOut = output;

	va_list body;

	va_start(body, fmt);
	pOut += vsprintf_s(output, OutputMaxLength, fmt, body);
	va_end(body);

	strcat_s(pOut, OutputMaxLength - (int)(pOut - output) - 1, "\n");

	OutputDebugStringA(output);
}
void GameServices::setGame(Game *g){
	mGame = g;
}
InputManager* GameServices::input() const {
	return mInputManager;
}

EventManager* GameServices::events() const {
	return mEventManager;
}

Ogre::Root* GameServices::ogreRoot() const {
	return mOgreRoot;
}

GameConfig* GameServices::gameConfig() const {
	return mGameConfig;
}