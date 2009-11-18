#include "RigidModelEntity.h"

class StartBoxEntity : public RigidModelEntity {
public:
	StartBoxEntity(GameServices *gs, OgreNewt::World *collisionWorld, Ogre::SceneNode *parentNode, const Ogre::Vector3 &pos, const Ogre::Vector3 &size, const Ogre::String &name, const Ogre::String &modelFile);
	~StartBoxEntity();

	void update();
	
	bool start();

	void onCollisionEntered(RigidModelEntity *colEntity);
private:
	bool mStart;
};