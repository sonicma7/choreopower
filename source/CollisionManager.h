#ifndef COLLISIONMANAGER_H
#define COLLISIONMANAGER_H

#include <list>
#include <map>

class ModelEntity;
class GameServices;
class EventManager;

class CollisionManager {
public:
	CollisionManager(GameServices *gs);
	~CollisionManager();
	
	void addMovingCollider(ModelEntity* ent);
	void addStaticCollider(ModelEntity* ent);

	void update();

private:
	bool checkForExistingCollision(ModelEntity* ent1, ModelEntity* ent2);
	void addCollision(ModelEntity* ent1, ModelEntity* ent2);
	void removeCollision(ModelEntity* ent1, ModelEntity* ent2);

	std::list<ModelEntity*> mMovingColliders;
	std::list<ModelEntity*> mStaticColliders;
	
	std::multimap<ModelEntity*, ModelEntity*> mCurrentCollisions;

	GameServices *mGameServices;
	EventManager *mEventManager;
};

#endif