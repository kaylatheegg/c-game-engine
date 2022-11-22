#pragma once
#define UTILS_GRAPH_H

typedef struct {
	int vertexIDs[2];
	float weight;
} GraphEdge;

typedef struct {
	size_t edgeCount;
	dynArray* edges; //dynamically allocated using the DynArray util
} GraphVertex;

typedef struct {
	size_t vertexCount;
	dynArray* vertices; //dynArray
} Graph;

Graph* createGraph(GraphVertex* vertices, size_t vertexCount);
int deleteGraph(Graph* intGraph);
int addVertex(Graph* intGraph, GraphEdge* edges, size_t edgeCount);
int removeVertex(Graph* intGraph, int vertexID);
int addEdge(Graph* intGraph, GraphEdge edge, int vertexID);
int removeEdge(Graph* intGraph, int vertexID, int edgeID);