#ifndef GRAPH_H
#define GRAPH_H

#include "Vector3.h"
#include <vector>

struct Edge
{
	unsigned source;
	unsigned end;
	float cost;
};

struct Node
{
	Vector3 pos;
	std::vector <unsigned> edges;
};

struct QueueNode
{
	unsigned node;
	float cost;
};

class Graph
{
public:
	Graph();
	~Graph();
	std::vector <Node*> m_nodes;
	std::vector <Edge*> m_edges;

	void AddNode(Vector3 _newPos);
	void AddEdge(unsigned _source, unsigned _end);
	void Generate(unsigned key);

	unsigned NearestNode(const Vector3 &pos);

private:
	unsigned m_key;

};

#endif // !GRAPH_H