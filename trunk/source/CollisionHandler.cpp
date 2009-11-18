#include "CollisionHandler.h"
#include "RigidModelEntity.h"
#include "GameServices.h"
#include "EventManager.h"
#include "GameServices.h"
#include "RigidModelDeletedEvent.h"

CollisionHandler::CollisionHandler(GameServices *gs)
:OgreNewt::ContactCallback() 
{
	mGameServices = gs;
	mGameServices->events()->addListener("PhysicsUpdateStarted", this, &CollisionHandler::beforePhysicsUpdate);
	mGameServices->events()->addListener("PhysicsUpdateEnded", this, &CollisionHandler::afterPhysicsUpdate);
	mGameServices->events()->addListener("RigidModelDeleted", this, &CollisionHandler::removeRigidModelEntity);
	mIterationsUnitlRemoved = 6;
}

CollisionHandler::~CollisionHandler() {
	mGameServices->events()->removeListener("PhysicsUpdateStarted", this);
	mGameServices->events()->removeListener("PhysicsUpdateEnded", this);
	mGameServices->events()->removeListener("RigidModelDeleted", this);
}

void CollisionHandler::beforePhysicsUpdate(GameEvent *evnt) {
	/*boost::property_map<UndirectedGraph, edge_colliding_t>::type collidingPropertyMap;

	collidingPropertyMap = boost::get(edge_colliding, mCollisionGraph);
	EdgeItr e, eEnd;
	
	//set all of the edges colliding property to false
	//when to objects collide, and they exist in the graph,
	//the colliding property will be set to true.
	for(boost::tie(e, eEnd) = boost::edges(mCollisionGraph); e != eEnd; ++e) {
		collidingPropertyMap[*e] -= 1;
	}*/
}

int CollisionHandler::userBegin() {
	return 1;
}

int CollisionHandler::userProcess() {
	//check to see if the vertices exist or not
	void* userData0 = m_body0->getUserData();
	void* userData1 = m_body1->getUserData();
	VertexMap::iterator itr0 = mVertexLookupList.find(userData0);
	VertexMap::iterator itr1 = mVertexLookupList.find(userData1);
	
	Vertex u, v;
	
	//if the vertices do not exist, add them
	if(itr0 == mVertexLookupList.end()) {
		u = boost::add_vertex(DataProp(userData0), mCollisionGraph);
		std::pair<VertexMap::iterator, bool> insertRet = mVertexLookupList.insert(std::pair<void*, Vertex>(userData0, u));
		itr0 = insertRet.first;
	}
	
	u = itr0->second;

	if(itr1 == mVertexLookupList.end()) {
		v = boost::add_vertex(DataProp(userData1), mCollisionGraph);
		std::pair<VertexMap::iterator, bool> insertRet = mVertexLookupList.insert(std::pair<void*, Vertex>(userData1, v));
		itr1 = insertRet.first;
	}
	
	v = itr1->second;

	//search the graph to see if the collision exists
	//i.e. check if an edge exists with the vertices representing
	//the two bodies
	std::pair<Edge, bool> e = boost::edge(u, v, mCollisionGraph);
	
	//if it does not exist that means this is a first time collision
	if(!e.second) {
		boost::add_edge(u, v, CollidingProp(mIterationsUnitlRemoved), mCollisionGraph);

		RigidModelEntity *ent0 = static_cast<RigidModelEntity*>(userData0);
		RigidModelEntity *ent1 = static_cast<RigidModelEntity*>(userData1);

		ent0->onCollisionEntered(ent1);
		ent1->onCollisionEntered(ent0);
	} else {
		boost::put(edge_colliding, mCollisionGraph, e.first, mIterationsUnitlRemoved);
	}

	return 1;
}

void CollisionHandler::userEnd() {

}

void CollisionHandler::afterPhysicsUpdate(GameEvent *evnt) {
	boost::property_map<UndirectedGraph, edge_colliding_t>::type collidingPropertyMap;
	collidingPropertyMap = boost::get(edge_colliding, mCollisionGraph);

	EdgeItr e, eEnd;
	
	std::vector<EdgeItr> edgesToRemove;
	
	//check what edges have the colliding property of false.
	//if the property is false they need to be removed.
	for(boost::tie(e, eEnd) = boost::edges(mCollisionGraph); e != eEnd; ++e) {
		if(collidingPropertyMap[*e] <= 0) {
			edgesToRemove.push_back(e);
		}
	}
	
	Vertex source, target;
	RigidModelEntity *ent0 = NULL, *ent1 = NULL;
	VertexItr v, vEnd;
	//remove all edges that need to be
	for(unsigned int i = 0; i < edgesToRemove.size(); ++i) {
		source = boost::source((*edgesToRemove[i]), mCollisionGraph);
		target = boost::target((*edgesToRemove[i]), mCollisionGraph);
		
		bool sourceFound = false, targetFound = false;
		for(boost::tie(v, vEnd) = boost::vertices(mCollisionGraph); v != vEnd; ++v) {
			if((*v) == source) {
				sourceFound = true;
			}

			if((*v) == target) {
				targetFound = true;
			}
		}
		
		if(sourceFound) {
			ent0 = static_cast<RigidModelEntity*>(boost::get(vertex_data, mCollisionGraph, source));
			ent0->onCollisionExited(ent1);
		}
		if(targetFound) {
			ent1 = static_cast<RigidModelEntity*>(boost::get(vertex_data, mCollisionGraph, target));
			ent1->onCollisionExited(ent0);
		}
		
		if(targetFound && sourceFound) {
			boost::remove_edge(*edgesToRemove[i], mCollisionGraph);
		}
	}
	
	collidingPropertyMap = boost::get(edge_colliding, mCollisionGraph);
	//changed updated property on edges to false
	for(boost::tie(e, eEnd) = boost::edges(mCollisionGraph); e != eEnd; ++e) {
		--collidingPropertyMap[*e];
	}
}

void CollisionHandler::removeRigidModelEntity(GameEvent *evnt) {
	void *ent = ((RigidModelDeletedEvent*)evnt)->getPointer();
	
	VertexMap::iterator itr = mVertexLookupList.find(ent);
	if(itr != mVertexLookupList.end()) {
		boost::remove_vertex(mVertexLookupList[ent], mCollisionGraph);
		mVertexLookupList.erase(mVertexLookupList.find(ent));
	}
}