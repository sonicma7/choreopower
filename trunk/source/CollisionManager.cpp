#include "CollisionManager.h"
#include "ModelEntity.h"
#include "GameServices.h"
#include "EventManager.h"

typedef std::multimap<ModelEntity*, ModelEntity*>::iterator CollisionMapItr;
typedef std::pair<CollisionMapItr, CollisionMapItr> CollisionMapItrPair;

CollisionManager::CollisionManager(GameServices *gs) {
	mGameServices = gs;
	mEventManager = mGameServices->events();
}

CollisionManager::~CollisionManager() {

}

void CollisionManager::addMovingCollider(ModelEntity* ent) {
	mMovingColliders.push_back(ent);
}

void CollisionManager::addStaticCollider(ModelEntity* ent) {
	mStaticColliders.push_back(ent);
}

void CollisionManager::update() {
	//check all moving colliders against static colliders
	//to see if there is a new collision
	for(std::list<ModelEntity*>::iterator movingItr = mMovingColliders.begin(); movingItr != mMovingColliders.end(); ++movingItr) {
		for(std::list<ModelEntity*>::iterator staticItr = mStaticColliders.begin(); staticItr != mStaticColliders.end(); ++staticItr) {
			//check if the moving object and static are already in collision
			bool alreadyInCollision = checkForExistingCollision(*movingItr, *staticItr);
			
			//check if they are currently colliding
			bool colliding = (*movingItr)->isColliding((*staticItr)->getAABB());
			
			
			if(!alreadyInCollision && colliding) {
				//add the collision pair to the set of current collisions
				addCollision(*movingItr, *staticItr);

				//throw entered collision event
				(*movingItr)->onCollisionEntered(*staticItr);
				(*staticItr)->onCollisionEntered(*movingItr);
			} else if(alreadyInCollision && !colliding) {
				//remove collision pair from the set of current collisions
				removeCollision(*movingItr, *staticItr);

				//throw exited collision event
				(*movingItr)->onCollisionExited(*staticItr);
				(*staticItr)->onCollisionExited(*movingItr);
			}
		}
	}
}

bool CollisionManager::checkForExistingCollision(ModelEntity* ent1, ModelEntity* ent2) {
	CollisionMapItrPair rangePair = mCurrentCollisions.equal_range(ent1);

	for(CollisionMapItr itr = rangePair.first; itr != rangePair.second; ++itr) {
		if(itr->second == ent2) {
			return true;
		}
	}

	return false;
}

void CollisionManager::addCollision(ModelEntity* ent1, ModelEntity* ent2) {
	mCurrentCollisions.insert(std::pair<ModelEntity*, ModelEntity*>(ent1, ent2));
}

void CollisionManager::removeCollision(ModelEntity* ent1, ModelEntity* ent2) {
	CollisionMapItrPair rangePair = mCurrentCollisions.equal_range(ent1);
	
	for(CollisionMapItr itr = rangePair.first; itr != rangePair.second; ++itr) {
		if(itr->second == ent2) {
			mCurrentCollisions.erase(itr);
			break;
		}
	}
}
