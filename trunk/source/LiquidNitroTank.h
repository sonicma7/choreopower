//tank moves along a scripted path to the right and down. It only starts moving down halfway through its x-movement
#include "GameServices.h"
#include "TriggerEvent.h"
#include "GameEvent.h"
#include "EventManager.h"
#include "PuzzleEntity.h"
class GameServices;
class TriggerEvent;
class EventManager;

class LiquidNitroTank : public PuzzleEntity {
public:
	LiquidNitroTank(GameServices *gs, OgreNewt::World* collisionWorld, Ogre::SceneNode *parentNode, 
		const Ogre::Vector3 &pos, const Ogre::Vector3 &size, 
		const Ogre::String &entityName, const Ogre::String &modelFile, 
		const Ogre::String &listenfor, const Ogre::Vector3 &dest, float xspeed, float yspeed);
	void update();
	void listenCallback(GameEvent *evnt);	//executed when a listened for event arrives
	void registerListener();
private:
	bool mActive;
	bool mFalling;
	float mX_Speed;
	float mY_Accel;
};