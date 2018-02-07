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

	AddNode(Vector3(10, 10, 0));
	AddNode(Vector3(10, 80, 0));
	AddNode(Vector3(80, 80, 0));
	AddNode(Vector3(80, 10, 0));
	//AddNode(Vector3(20, 11, 0));
	//AddNode(Vector3(23, 3, 0));

	AddEdge(0, 1);
	AddEdge(1, 2);
	AddEdge(2, 3);
	AddEdge(3, 0);
}

unsigned Graph::NearestNode(const Vector3 & pos)
{
	unsigned result = 0;

	float displacement = FLT_MAX;

	for (unsigned i = 0; i < m_nodes.size(); ++i)
	{
		float tempdisplace = (m_nodes[i]->pos - pos).Length();
		if (displacement < tempdisplace)
		{

		}
	}

	return result;
}
