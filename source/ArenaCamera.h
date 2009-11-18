#include <OgreCamera.h>
#include <OgreSceneNode.h>

class ArenaCamera {
public:
	ArenaCamera(Ogre::SceneNode *target1, Ogre::SceneNode *target2, Ogre::Camera *cam);
	~ArenaCamera();
	
	void setMinDistanceAway(float dist);

	void update();

private:
	Ogre::SceneNode *mTarget1;
	Ogre::SceneNode *mTarget2;
	Ogre::Camera *mCamera;
	float mMinDist;
};