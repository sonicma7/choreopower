#pragma once
#include "TriggerEntity.h"
#include <Ogre.h>
#include <OgreNewt.h>

class GameServices;
class TriggerEvent;
class EventManager;
class GameEvent;

class PuzzleEntity : public TriggerEntity {
public:
	PuzzleEntity(GameServices *gs, OgreNewt::World* collisionWorld, Ogre::SceneNode *parentNode, 
					const Ogre::Vector3 &pos, const Ogre::Vector3 &size, 
					const Ogre::String &entityName, const Ogre::String &modelFile, 
					const Ogre::String &triggername, const Ogre::String &listenfor, const int triggerID);
	PuzzleEntity(GameServices *gs, OgreNewt::World* collisionWorld, Ogre::SceneNode *parentNode, 
					const Ogre::Vector3 &pos, const Ogre::Vector3 &size, 
					const Ogre::String &entityName, const Ogre::String &modelFile, 
					const Ogre::String &triggername, const int triggerID);
	PuzzleEntity(GameServices *gs, OgreNewt::World* collisionWorld, Ogre::SceneNode *parentNode, 
					const Ogre::Vector3 &pos, const Ogre::Vector3 &size, 
					const Ogre::String &entityName, const Ogre::String &modelFile, 
					const Ogre::String &listenfor);
	~PuzzleEntity();
	virtual const std::string getType() const;
	virtual void throwTrigger();
	virtual void listenCallback(GameEvent *evnt);	//executed when a listened for event arrives
	virtual void registerListener();	//register an event listener
	virtual void onCollisionEntered(RigidModelEntity *colEntity);
	virtual void onCollisionExited(RigidModelEntity *colEntity);
protected:
	bool mThrowsTriggers;
	bool mRegistersListener;
	std::string mTriggerName;
	std::string mListenFor;
};