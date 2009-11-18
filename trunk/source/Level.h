#pragma once
#include "newton.h"
#include "PlayerEntity.h"

enum LevelState {
	LS_Reset = 0,
	LS_Arena,
	LS_Two,
	LS_One,
	LS_Keeper,
};

class Level {
public:
	virtual ~Level() {}
	virtual LevelState update() = 0;

	void pause(bool val) { mPaused = val; }
	bool paused() const { return mPaused; }

	virtual void loadLevel() = 0;
	virtual void unloadLevel() = 0;
	LevelState getLevel() { return mNextLevel; }
	virtual void setLevel(LevelState newLevel) { mNextLevel = newLevel; }
	virtual void getPlayers(PlayerEntity* p1, PlayerEntity* p2) = 0;
	bool loaded() const { return mLoaded; }

protected:
	virtual void initCamera() = 0;
	virtual void initLighting() = 0;
	virtual void initScene() = 0;
	void initPhysics();

	bool mPaused;
	bool mLoaded;
	LevelState mRunning;
	LevelState mNextLevel;

	NewtonWorld* mNewtonWorld;
};