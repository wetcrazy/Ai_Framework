#include "SceneGraph.h"
#include "GL\glew.h"
#include "Application.h"
#include <sstream>

#define SPAWN_TIMER 3

SceneGraph::SceneGraph()
{
}

SceneGraph::~SceneGraph()
{
}

void SceneGraph::Init()
{
	SceneBase::Init();

	//Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	//Physics code here
	m_speed = 1.f;

	Math::InitRNG();

	m_graph.Generate(0);

	spawntimerLab11 = 0.0f;
}

void SceneGraph::InitPath()
{
	std::vector<Vector3> left, middle, right;

	left.push_back(m_graph.m_nodes[0]->pos);
	left.push_back(m_graph.m_nodes[1]->pos);
	left.push_back(m_graph.m_nodes[4]->pos);
	left.push_back(m_graph.m_nodes[7]->pos);
	left.push_back(m_graph.m_nodes[10]->pos);

	middle.push_back(m_graph.m_nodes[0]->pos);
	middle.push_back(m_graph.m_nodes[2]->pos);
	middle.push_back(m_graph.m_nodes[5]->pos);
	middle.push_back(m_graph.m_nodes[8]->pos);
	middle.push_back(m_graph.m_nodes[10]->pos);

	right.push_back(m_graph.m_nodes[0]->pos);
	right.push_back(m_graph.m_nodes[3]->pos);
	right.push_back(m_graph.m_nodes[6]->pos);
	right.push_back(m_graph.m_nodes[9]->pos);
	right.push_back(m_graph.m_nodes[10]->pos);


}

void SceneGraph::AssignPath(GameObject *go)
{
	//switch (go->Lab11_factionType)
	switch (go->Asign2_factionType)
	{
	case FACTION_RED:
	{
		int rangen = Math::RandIntMinMax(0, 2); // this rand need change according to graph
		go->gPath.push_back(m_paths[rangen]);
		break;
	}
	case FACTION_BLUE:
	{
		int rangen = Math::RandIntMinMax(0, 2); // this rand need change according to graph
		go->gPath.push_back(m_paths[rangen]);
		break;
	}
	case FACTION_NEUTRAL:
		break;
	}
	
}

GameObject* SceneGraph::FetchGO(GameObject::GAMEOBJECT_TYPE type)
{
	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (!go->active && go->type == type)
		{
			go->active = true;
			++m_objectCount;
			return go;
		}
	}
	for (unsigned i = 0; i < 5; ++i)
	{
		GameObject *go = new GameObject(type);
		m_goList.push_back(go);
	}
	return FetchGO(type);
}

void SceneGraph::Update(double dt)
{
	SceneBase::Update(dt);
	//spawntimerLab11 += dt;
	Asign2_spawntimer += dt;

	//Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	if (Application::IsKeyPressed(VK_OEM_MINUS))
	{
		m_speed = Math::Max(0.f, m_speed - 0.1f);
	}
	if (Application::IsKeyPressed(VK_OEM_PLUS))
	{
		m_speed += 0.1f;
	}
	if (Application::IsKeyPressed('N'))
	{
	}
	if (Application::IsKeyPressed('M'))
	{
	}
	if (Application::IsKeyPressed(VK_RETURN))
	{
	}
	if (Application::IsKeyPressed('R'))
	{
	}

	//Input Section
	static bool bLButtonState = false;
	if (!bLButtonState && Application::IsMousePressed(0))
	{
		bLButtonState = true;
		std::cout << "LBUTTON DOWN" << std::endl;
		double x, y;
		Application::GetCursorPos(&x, &y);
		int w = Application::GetWindowWidth();
		int h = Application::GetWindowHeight();
		float posX = static_cast<float>(x) / w * m_worldWidth;
		float posY = (h - static_cast<float>(y)) / h * m_worldHeight;
		if (posX < m_worldHeight && posY < m_worldHeight)
		{
		}
	}
	else if (bLButtonState && !Application::IsMousePressed(0))
	{
		bLButtonState = false;
		std::cout << "LBUTTON UP" << std::endl;
	}
	static bool bRButtonState = false;
	if (!bRButtonState && Application::IsMousePressed(1))
	{
		bRButtonState = true;
		std::cout << "RBUTTON DOWN" << std::endl;
	}
	else if (bRButtonState && !Application::IsMousePressed(1))
	{
		bRButtonState = false;
		std::cout << "RBUTTON UP" << std::endl;
	}
	static bool bSpaceState = false;
	if (!bSpaceState && Application::IsKeyPressed(VK_SPACE))
	{
		bSpaceState = true;
	}
	else if (bSpaceState && !Application::IsKeyPressed(VK_SPACE))
	{
		bSpaceState = false;
	}	

	// Key Spawn

	static bool b_keyState = false;
	if (!b_keyState && Application::IsKeyPressed('Z'))
	{
		b_keyState = true;

		GameObject *go = FetchGO(GameObject::GO_RED);
		go->Asign2_hp = 1.0f;
		go->Asign2_factionType = FACTION_RED;
		go->Asign2_unitType = UNIT_SCISSOR;
		AssignPath(go);
		go->pos.Set(go->path.front().x, go->path.front().y, 0.0f);
		go->target = go->pos;
	}
	else if (!b_keyState && Application::IsKeyPressed('X'))
	{
		b_keyState = true;

		GameObject *go = FetchGO(GameObject::GO_RED);
		go->Asign2_hp = 1.0f;
		go->Asign2_factionType = FACTION_RED;
		go->Asign2_unitType = UNIT_PAPER;
		AssignPath(go);
		go->pos.Set(go->path.front().x, go->path.front().y, 0.0f);
		go->target = go->pos;
	}
	else if (!b_keyState && Application::IsKeyPressed('C'))
	{
		b_keyState = true;

		GameObject *go = FetchGO(GameObject::GO_RED);
		go->Asign2_hp = 1.0f;
		go->Asign2_factionType = FACTION_RED;
		go->Asign2_unitType = UNIT_ROCK;
		AssignPath(go);
		go->pos.Set(go->path.front().x, go->path.front().y, 0.0f);
		go->target = go->pos;
	}
	else if (b_keyState)
	{
		b_keyState = false;
	}

	// Spawner
	/*if (spawntimerLab11 >= SPAWN_TIMER)
	{
		{
			GameObject *go = FetchGO(GameObject::GO_RED);
			go->Lab11_hp = 1.0f;
			go->Lab11_range = 1.0f;
			go->Lab11_damage = 1.0f;
			go->Lab11_actionTime = 1.0f;
			AssignPath(go);
			go->pos.Set(go->path.front().x, go->path.front().y, 0.0f);
			go->target = go->pos;
		}
	}*/
	

	// Combat
	/*for (auto &go : m_goList)
	{
		if (go->Lab11_factionType == NULL || go->Lab11_factionType == 0)
			return;
		GameObject *go1 = (GameObject*) go;
		for (auto goB : m_goList)
		{
			if (go->Lab11_factionType == NULL || go->Lab11_factionType == 0)
				return;
			GameObject *go2 = (GameObject*) go;
			if (go1->pos.x + go1->Lab11_range <= go2->pos.x && go1->pos.y + go1->Lab11_range <= go2->pos.y)
			{
				go2->Lab11_hp -= go1->Lab11_damage;
			}
		}
	}*/

	// Combat
	for (auto goA : m_goList)
	{
		if (goA->active)
			continue;

		if (goA->Asign2_factionType != NULL || goA->Lab11_factionType != 0 || goA->Asign2_unitType == NULL) // check for faction and type
			continue;
		
		for (auto goB : m_goList)
		{
			if (goB->active)
				continue;

			if (goB->Asign2_factionType != NULL || goB->Asign2_factionType != 0 || goB->Asign2_unitType == NULL) // check for faction  and type
				continue;
			
			if (goA->Asign2_factionType != goB->Asign2_factionType) // check for same faction, if not then continue
				continue;

			if ((goA->pos - goB->pos).Length() <= 1) // when they touch each other
			{
				// Combat logic
				if (goA->Asign2_unitType == UNIT_SCISSOR && goB->Asign2_unitType == UNIT_PAPER)
				{
					goB->active = false;
				}
				else if (goA->Asign2_unitType == UNIT_SCISSOR && goB->Asign2_unitType == UNIT_ROCK)
				{
					goA->active = false;
				}
				else if (goA->Asign2_unitType == UNIT_PAPER && goB->Asign2_unitType == UNIT_SCISSOR)
				{
					goA->active = false;
				}
				else if (goA->Asign2_unitType == UNIT_PAPER && goB->Asign2_unitType == UNIT_ROCK)
				{
					goB->active = false;
				}
				else if (goA->Asign2_unitType == UNIT_ROCK && goB->Asign2_unitType == UNIT_SCISSOR)
				{
					goB->active = false;
				}
				else if (goA->Asign2_unitType == UNIT_ROCK && goB->Asign2_unitType == UNIT_PAPER)
				{
					goA->active = false;
				}
				else if (goA->Asign2_unitType == goB->Asign2_unitType)
				{
					goA->active = false;
					goB->active = false;
				}
			}
			
		}
	}

	// MOVEMENT 
	static const float NPC_VELOCITY = 10.f;
	for (auto go : m_goList)
	{
		if (go->active && go->type == GameObject::GO_NPC)
		{
			if ((go->pos - go->target).Length() < 1.f)
			{
				/*if (!go->gStack.empty())
				{
					DFSOnce(go);
					go->target =
						m_graph.m_nodes[go->currNode]->pos;
				}*/
				if (!go->gPath.empty())
				{
					go->target = go->gPath[0];
					go->gPath.erase(go->gPath.begin());
				}
				/*else
				{
					GoRandomAdjacent(go);
				}*/
			}
			else
			{
				go->pos += (go->target - go->pos).Normalized()
					* NPC_VELOCITY * m_speed * static_cast<float>(dt);
			}
		}
	}
}


void SceneGraph::RenderGO(GameObject *go)
{
	switch (go->type)
	{
	case GameObject::GO_NPC:
		break;
	case GameObject::GO_RED:
		break;
	case GameObject::GO_BLUE:
		break;
	case GameObject::GO_NETURAL:
		break;
	}
}

void SceneGraph::GoRandomAdjacent(GameObject * go)
{
	unsigned nodeID = m_graph.NearestNode(go->pos);
	Node* currNode = m_graph.m_nodes[nodeID];
	unsigned edgeID = currNode->edges[Math::RandIntMinMax(0, currNode->edges.size())];
	Edge* edge = m_graph.m_edges[edgeID];
	Node* nextNode = m_graph.m_nodes[edge->end];

	go->gPath.push_back(nextNode->pos);
}

void SceneGraph::DFSOnce(GameObject * go)
{
	go->gStack.push_back(go->currNodeID);
	go->visited[go->currNodeID] = true;
	
	Node* currNode = m_graph.m_nodes[go->currNodeID];
	for (auto edgeID: currNode->edges)
	{
		Edge* edge = m_graph.m_edges[edgeID];		
		
		if (go->visited[edge[edgeID].end] == false)
		{
			go->currNodeID = edge[edgeID].end;
			return;
		}
	}
	go->gStack.pop_back();
	if (!go->gStack.empty())
	{
		go->curr = go->gStack.back();
		go->gStack.pop_back();
		return;
	}
}

void SceneGraph::RenderFaction()
{
	for (auto i : m_goList)
	{
		if (i->Lab11_factionType == NULL || i->Lab11_factionType == 0)
			return;

		switch (i->Lab11_factionType)
		{
		case FACTION_BLUE:
			modelStack.PushMatrix();
			modelStack.Translate(i->pos.x, i->pos.y, i->pos.z);
			modelStack.Scale(1, 1, 1);
			RenderMesh(meshList[GEO_BLUE], false);
			modelStack.PopMatrix();
			break;
		case FACTION_RED:
			modelStack.PushMatrix();
			modelStack.Translate(i->pos.x, i->pos.y, i->pos.z);
			modelStack.Scale(1, 1, 1);
			RenderMesh(meshList[GEO_RED], false);
			modelStack.PopMatrix();
			break;
		case FACTION_NEUTRAL:
			modelStack.PushMatrix();
			modelStack.Translate(i->pos.x, i->pos.y, i->pos.z);
			modelStack.Scale(1, 1, 1);
			RenderMesh(meshList[GEO_NEUTRAL], false);
			modelStack.PopMatrix();
			break;
		default:
			break;
		}
					
	}
}

void SceneGraph::RenderGraph()
{
	for (auto &i : m_graph.m_nodes)
	{
		modelStack.PushMatrix();
		modelStack.Translate(i->pos.x,i->pos.y, i->pos.z);
		modelStack.Scale(1, 1, 1);
		RenderMesh(meshList[GEO_NODE], false);
		modelStack.PopMatrix();
	}
	for (auto &i : m_graph.m_edges)
	{
		Vector3 dir = m_graph.m_nodes[i->end]->pos - m_graph.m_nodes[i->source]->pos;

		modelStack.PushMatrix();
		modelStack.Translate(m_graph.m_nodes[i->source]->pos.x, m_graph.m_nodes[i->source]->pos.y, 1.f);
		modelStack.Rotate(Math::RadianToDegree(atan2(dir.y, dir.x)), 0, 0, 1);
		modelStack.Scale((m_graph.m_nodes[i->source]->pos - m_graph.m_nodes[i->end]->pos).Length(), 1, 1);
		RenderMesh(meshList[GEO_LINE], false);
		modelStack.PopMatrix();
	}
	
}


void SceneGraph::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Projection matrix : Orthographic Projection
	Mtx44 projection;
	projection.SetToOrtho(0, m_worldWidth, 0, m_worldHeight, -10, 10);
	projectionStack.LoadMatrix(projection);

	// Camera matrix
	viewStack.LoadIdentity();
	viewStack.LookAt(
		camera.position.x, camera.position.y, camera.position.z,
		camera.target.x, camera.target.y, camera.target.z,
		camera.up.x, camera.up.y, camera.up.z
		);
	// Model matrix : an identity matrix (model will be at the origin)
	modelStack.LoadIdentity();

	RenderMesh(meshList[GEO_AXES], false);
	
	modelStack.PushMatrix();
	modelStack.Translate(m_worldHeight * 0.5f, m_worldHeight * 0.5f, -1.f);
	modelStack.Scale(m_worldHeight, m_worldHeight, m_worldHeight);
	RenderMesh(meshList[GEO_WHITEQUAD], false);
	modelStack.PopMatrix();

	RenderGraph();
	RenderFaction();

	//On screen text
	std::ostringstream ss;
	ss.precision(3);
	ss << "Speed:" << m_speed;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 50, 6);

	ss.str("");
	ss.precision(5);
	ss << "FPS:" << fps;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 50, 3);

	ss.str("");
	ss << "Graph " << 0;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 50, 0);
}

void SceneGraph::Exit()
{
	SceneBase::Exit();
	//Cleanup GameObjects
	while (m_goList.size() > 0)
	{
		GameObject *go = m_goList.back();
		delete go;
		m_goList.pop_back();
	}
}
