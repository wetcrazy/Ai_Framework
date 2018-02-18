#ifndef SCENE_GRAPH_H
#define SCENE_GRAPH_H

#include "GameObject.h"
#include <vector>
#include "SceneBase.h"
#include "Maze.h"
#include <queue>
#include "Graph.h"
#include "SceneData.h"

class SceneGraph : public SceneBase
{
public:
	SceneGraph();
	~SceneGraph();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void RenderGO(GameObject *go);
	GameObject* FetchGO(GameObject::GAMEOBJECT_TYPE type);

	void RenderGraph();
	
	void GoRandomAdjacent(GameObject *go);
	void DFSOnce(GameObject *go);
	bool AStar(GameObject *go, unsigned start, unsigned end);

	enum UNITS
	{
		UNIT_CREEP = 1,
		UNIT_TOWER,
		UNIT_SCISSOR,
		UNIT_ROCK,
		UNIT_PAPER,
		UNIT_TOTAL,
	};

	enum FACTIONS
	{
		FACTION_BLUE = 1,
		FACTION_RED,
		FACTION_NEUTRAL,
		FACTION_TOTAL,
	};

	void InitPath();
	void AssignPath(GameObject *go);
	void RenderFaction();

protected:

	std::vector<GameObject *> m_goList;
	std::vector <std::vector<Vector3>> m_paths;
	//std::vector <unsigned int> m_paths;
	float m_speed;
	float m_worldWidth;
	float m_worldHeight;
	int m_objectCount;

	Graph m_graph;
	unsigned m_graphKey;

	float spawntimerLab11;

	float Asign2_spawntimer;
};

#endif