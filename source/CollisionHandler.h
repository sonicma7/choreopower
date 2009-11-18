#pragma once

#include <OgreNewt_ContactCallback.h>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/properties.hpp>
#include <map>

class GameServices;
class GameEvent;

enum edge_colliding_t { edge_colliding };
enum vertex_data_t { vertex_data };
namespace boost {
	BOOST_INSTALL_PROPERTY(edge, colliding);
	BOOST_INSTALL_PROPERTY(vertex, data);
}

typedef boost::property<edge_colliding_t, int> CollidingProp;
typedef boost::property<vertex_data_t, void*> DataProp;
typedef boost::adjacency_list<boost::vecS, boost::listS, boost::undirectedS, DataProp, CollidingProp> UndirectedGraph;
typedef boost::graph_traits<UndirectedGraph>::edge_descriptor Edge;
typedef boost::graph_traits<UndirectedGraph>::edge_iterator EdgeItr;
typedef boost::graph_traits<UndirectedGraph>::vertex_descriptor Vertex;
typedef boost::graph_traits<UndirectedGraph>::vertex_iterator VertexItr;
typedef std::map<void*, Vertex> VertexMap;

class CollisionHandler : public OgreNewt::ContactCallback {
public:
	CollisionHandler(GameServices* gs);
	~CollisionHandler();

	void beforePhysicsUpdate(GameEvent *evnt);

	int userBegin();
	int userProcess();
	void userEnd();

	void afterPhysicsUpdate(GameEvent *evnt);

	void removeRigidModelEntity(GameEvent *evnt);

private:
	UndirectedGraph mCollisionGraph;
	GameServices* mGameServices;
	std::map<void*, Vertex> mVertexLookupList;
	unsigned int mIterationsUnitlRemoved;
};