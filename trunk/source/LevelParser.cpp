#include "LevelParser.h"
#include "RigidModelEntity.h"
#include "Ogre.h"
#include <vector>
#include <sstream>
#include "ticpp.h"


void LevelParser::parseLevel(std::string filename){
	mFilename = filename;
	ticpp::Document* doc;
	doc = new ticpp::Document();
	try{

	doc->LoadFile(mFilename);

	
	ticpp::Element* sceneElement = doc->FirstChildElement()->NextSiblingElement();

	//ticpp::Element* sceneManagerElement = sceneElement->FirstChildElement("SCENEMANAGER");
	ticpp::Iterator< ticpp::Element > sceneNodeItr( "OBJECT" );
	//ticpp::Iterator< ticpp::Element > entityItr;
	//ticpp::Iterator< ticpp::Element > subEntityItr;
	for ( sceneNodeItr = sceneNodeItr.begin( sceneElement ); sceneNodeItr != sceneNodeItr.end(); sceneNodeItr++ ){
		std::string pos;
		std::string orient;
		std::string scale;
		std::string parentNode;
		std::string shadowtest;
		std::string visibletest;
		std::string killtest;
		int index = 0;
		std::string type;
		sceneNodeItr->GetAttribute("type", &type);

		EntityProperties ent;

		if(type == "Node Object"){
		
			sceneNodeItr->GetAttribute("position", &pos);
			sceneNodeItr->GetAttribute("orientation", &orient);
			sceneNodeItr->GetAttribute("scale", &scale);

			ent.Position = toVector(pos);
			ent.Scale = toVector(scale);
			ent.Orientation = toQuat(orient);

			//entityItr = sceneNodeItr->FirstChildElement();

			sceneNodeItr->GetAttribute("name", &ent.Name);
			
			entityList.push_back(ent);
		}
		if(type == "Entity Object"){
			sceneNodeItr->GetAttribute("parentnode", &parentNode);
			for(unsigned int i = 0; i<entityList.size();i++){
				if(parentNode == entityList[i].Name){
					index = i;
				}
			}
			
			sceneNodeItr->GetAttribute("kill", &killtest);
			if(killtest == "true"){
				entityList[index].Kill = true;
			}
			else{
				entityList[index].Kill = false;
			}

			sceneNodeItr->GetAttribute("meshfile", &entityList[index].Meshfile);
			sceneNodeItr->GetAttribute("shadows", &shadowtest);
			if(shadowtest == "true"){
				entityList[index].Shadows = true;
			}
			else{
				entityList[index].Shadows = false;
			}

			sceneNodeItr->GetAttribute("subentity0.material", &entityList[index].Materialfile);
			sceneNodeItr->GetAttribute("subentity0.visible", &visibletest);
			if(visibletest == "true"){
				entityList[index].Visible = true;
			}
			else{
				entityList[index].Visible = false;
			}
		}
	}
	} catch(ticpp::Exception &ex) {
		std::string what;
		what = ex.what();
	}
}

Ogre::Vector3 LevelParser::toVector(std::string num){
	float buf;
	std::stringstream ss(num);
	

	std::vector<float> tokens;
	while (ss >> buf){
		tokens.push_back(buf);
	}

	return Ogre::Vector3(tokens[0], tokens[1], tokens[2]);
}

Ogre::Quaternion LevelParser::toQuat(std::string num){
	float buf;
	std::stringstream ss(num);


	std::vector<float> tokens;
	while (ss >> buf){
		tokens.push_back(buf);
	}

	return Ogre::Quaternion(tokens[0], tokens[1], tokens[2], tokens[3]);
}

std::vector<EntityProperties> LevelParser::getVect(){
	return entityList;
}