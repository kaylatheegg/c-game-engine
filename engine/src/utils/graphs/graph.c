#include "engine.h"
/*
typedef struct GraphEdge {
	int vertexIDs[2];
	float weight;
}

typedef struct GraphVertex {
	size_t edgeCount;
	GraphEdge* edges; //dynamically allocated using the DynArray util
}

typedef struct Graph {
	size_t vertexCount;
	GraphVertex* vertices;
}
*/



Graph* createGraph(GraphVertex* vertices, size_t vertexCount) {
	UNUSED(vertices);
	UNUSED(vertexCount);
	return NULL;
} 

int addVertex(Graph* intGraph, GraphEdge* edges, size_t edgeCount) {
	UNUSED(intGraph);
	UNUSED(edges);
	UNUSED(edgeCount);
	return 0;
}

//hm. dynArray's ids will shift after the vertex is removed, completely destroying the graph.
//it would be better here to insert a "null" vertex, so that the graph could write over this vertex if needed
//idk
//this is a bug to fix when i need to

int removeVertex(Graph* intGraph, int vertexID) {
	UNUSED(intGraph);
	UNUSED(vertexID);
	return 0;
}

int addEdge(Graph* intGraph, GraphEdge edge, int vertexID) {
	UNUSED(intGraph);
	UNUSED(edge);
	UNUSED(vertexID);
	return 0;
}

int removeEdge(Graph* intGraph, int vertexID, int edgeID) {
	UNUSED(intGraph);
	UNUSED(vertexID);
	UNUSED(edgeID);
	return 0;
}

int deleteGraph(Graph* intGraph) {
	UNUSED(intGraph);
	return 0;
}

int drawGraph(Graph* intGraph, int x, int y) {
	UNUSED(intGraph);
	UNUSED(x);
	UNUSED(y);
	return 0;
}

Graph* nearestNeighbour(Graph* intGraph) { //generates a tree from the graph, starting from a random vertex
	UNUSED(intGraph);
	return NULL;
}

//horrible bug found!
/*

the entire graph data structure does not allow for backwards traversal through all nodes
because im fucking stupid and wrote addEdge wrong.
the entire graph system should be rewritten but for now my brain is VERY tired
*/

