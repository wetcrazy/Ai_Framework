#include "Graph.h"

Graph::Graph()
{
}

Graph::~Graph()
{
	for (unsigned index = 0; index < m_nodes.size(); ++index)
	{
		m_nodes.pop_back();
	}
	for (unsigned index = 0; index < m_edges.size(); ++index)
	{
		m_edges.pop_back();
	}
}

void Graph::AddNode(Vector3 _newPos)
{
	Node *temp = new Node();
	temp->pos = _newPos;
	m_nodes.push_back(temp);
}

void Graph::AddEdge(unsigned _source, unsigned _end)
{
	Edge *temp = new Edge();
	temp->source = _source;
	temp->end = _end;

	float displacement = (m_nodes[_source]->pos - m_nodes[_end]->pos).LengthSquared();
	temp->cost = displacement;

	m_edges.push_back(temp);

	m_nodes[_source]->edges.push_back(m_edges.size() - 1);
}

void Graph::Generate(unsigned key)
{
	//float randX, randY;
	srand(key);

	AddNode(Vector3(50, 0, 0)); // 0
	AddNode(Vector3(25, 25, 0)); // 1
	AddNode(Vector3(50, 25, 0)); // 2
	AddNode(Vector3(75, 25, 0)); // 3
	AddNode(Vector3(25, 50, 0)); // 4
	AddNode(Vector3(50, 50, 0)); // 5
	AddNode(Vector3(75, 50, 0)); // 6
	AddNode(Vector3(25, 75, 0)); // 7
	AddNode(Vector3(50, 75, 0)); // 8
	AddNode(Vector3(75, 75, 0)); // 9
	AddNode(Vector3(50, 100, 0)); // 10

	// Left
	AddEdge(0, 1);
	AddEdge(1, 4);
	AddEdge(4, 7);
	AddEdge(7, 10);
	
	// Middle
	AddEdge(0, 2);
	AddEdge(2, 5);
	AddEdge(5, 8);
	AddEdge(8, 10);

	// Right
	AddEdge(0, 3);
	AddEdge(3, 6);
	AddEdge(6, 9);
	AddEdge(9, 10);
}

unsigned Graph::NearestNode(const Vector3 & pos)
{
	unsigned result = 0;

	float displacement = FLT_MAX;

	for (unsigned i = 0; i < m_nodes.size(); ++i)
	{
		float tempdisplace = (m_nodes[i]->pos - pos).Length();
		if (displacement > tempdisplace)
		{
			displacement = tempdisplace;
			result = i;
		}
	}

	return result;
}
