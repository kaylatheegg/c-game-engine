#pragma once
#define UTILS_GRAPH_H

typedef struct {
	int vertexIDs[2];
	float weight;
} GraphEdge;

typedef struct {
	int id;
} GraphVertex;

typedef struct {
	size_t vertexCount;
	size_t edgeCount;
	dynArray* vertices; //dynArray
	dynArray* edges;
} Graph;

Graph* createGraph(GraphVertex* vertices, size_t vertexCount);
int deleteGraph(Graph* intGraph);
int addVertex(Graph* intGraph, GraphEdge* edges, size_t edgeCount);
int removeVertex(Graph* intGraph, int vertexID);
int addEdge(Graph* intGraph, GraphEdge edge, int vertexID);
int removeEdge(Graph* intGraph, int vertexID, int edgeID);

int drawGraph(Graph* intGraph, int x, int y);