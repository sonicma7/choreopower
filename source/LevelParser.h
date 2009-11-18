#pragma once

#include <ticpp.h>
#include <Ogre.h>
#include <vector>

class EntityProperties{
public:
	Ogre::String Name;
	Ogre::Vector3 Position;
	Ogre::Quaternion Orientation;
	Ogre::Vector3 Scale;
	Ogre::String Meshfile;
	bool Shadows;
	Ogre::String Materialfile;
	bool Visible;
	bool Kill;
};



class LevelParser{
public:
	void parseLevel(std::string filename);

	Ogre::Vector3 toVector(std::string num);
	Ogre::Quaternion toQuat(std::string num);

	std::vector<EntityProperties> getVect();

protected:
	std::string mFilename;
	std::vector<EntityProperties> entityList;
};






