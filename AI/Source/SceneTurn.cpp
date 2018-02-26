#include "SceneTurn.h"
#include "GL\glew.h"
#include "Application.h"
#include <sstream>

#define TURN_TIME 5
#define MAX 20

SceneTurn::SceneTurn()
{
}

SceneTurn::~SceneTurn()
{
}

void SceneTurn::Init()
{
	SceneBase::Init();

	//Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	//Physics code here
	m_speed = 1.f;

	Math::InitRNG();

	m_noGrid = 12;
	m_gridSize = m_worldHeight / m_noGrid;
	m_gridOffset = m_gridSize / 2;

	m_start.Set(0, 0);
	m_mazeKey = 0;
	m_wallLoad = 0.3f;
	m_maze.Generate(m_mazeKey, m_noGrid, m_start, m_wallLoad); //Generate new maze
	m_myGrid.resize(m_noGrid * m_noGrid);
	m_visited.resize(m_noGrid * m_noGrid);
	m_previous.resize(m_noGrid * m_noGrid);
	std::fill(m_myGrid.begin(), m_myGrid.end(), Maze::TILE_FOG);
	std::fill(m_visited.begin(), m_visited.end(), false);
	m_myGrid[m_start.y * m_noGrid + m_start.x] = Maze::TILE_EMPTY;
	DFS(m_start);

	m_turn = 0;
	m_timer = 0.f;

	int x_here, y_here;
	GameObject *go = FetchGO(GameObject::GO_NPC);
	go->grid.resize(m_noGrid * m_noGrid);
	go->visited.resize(m_noGrid * m_noGrid);
	std::fill(go->grid.begin(), go->grid.end(), Maze::TILE_FOG);
	std::fill(go->visited.begin(), go->visited.end(), false);
	do {
		x_here = Math::RandIntMinMax(0, m_noGrid - 1);
		y_here = Math::RandIntMinMax(0, m_noGrid - 1);
	} while (m_maze.m_grid[go->curr.y * m_noGrid + go->curr.x] != Maze::TILE_EMPTY);
	go->curr.x = x_here;
	go->curr.y = y_here;
	go->stack.push_back(go->curr);
	go->scale.x = m_gridSize;
	go->scale.y = m_gridSize;
	go->scale.z = m_gridSize;
	go->grid[go->curr.y * m_noGrid + go->curr.x] = Maze::TILE_EMPTY;
}

//GameObject* SceneTurn::FetchGO()
//{
//	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
//	{
//		GameObject *go = (GameObject *)*it;
//		if (!go->active)
//		{
//			go->active = true;
//			++m_objectCount;
//			return go;
//		}
//	}
//	for (unsigned i = 0; i < 10; ++i)
//	{
//		GameObject *go = new GameObject(GameObject::GO_NONE);
//		m_goList.push_back(go);
//	}
//	return FetchGO();
//}

GameObject* SceneTurn::FetchGO(GameObject::GAMEOBJECT_TYPE typeValue)
{
	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (!go->active && typeValue == GameObject::GO_NPC)
		{
			go->active = true;
			go->type = typeValue;
			++m_objectCount;
			return go;
		}
	}
	for (unsigned i = 0; i < 10; ++i)
	{
		GameObject *go = new GameObject(typeValue);
		m_goList.push_back(go);
	}
	return FetchGO(typeValue);
}

void SceneTurn::DFS(MazePt curr)
{
	m_visited[curr.y * m_noGrid + curr.x] = true;
	//UP
	if(curr.y < m_noGrid - 1)
	{
		MazePt next(curr.x, curr.y + 1);
		if (!m_visited[next.y * m_noGrid + next.x])
		{
			if (m_maze.Move(Maze::DIR_UP) == true)
			{
				m_myGrid[next.y * m_noGrid + next.x] = Maze::TILE_EMPTY;
				DFS(next);
				m_maze.Move(Maze::DIR_DOWN);
			}
			else
			{
				m_myGrid[next.y * m_noGrid + next.x] = Maze::TILE_WALL;
			}
		}
	}
	//DOWN
	if (curr.y > 0)
	{
		MazePt next(curr.x, curr.y - 1);
		if (!m_visited[next.y * m_noGrid + next.x])
		{
			if (m_maze.Move(Maze::DIR_DOWN) == true)
			{
				m_myGrid[next.y * m_noGrid + next.x] = Maze::TILE_EMPTY;
				DFS(next);
				m_maze.Move(Maze::DIR_UP);
			}
			else
			{
				m_myGrid[next.y * m_noGrid + next.x] = Maze::TILE_WALL;
			}
		}
	}
	//LEFT
	if (curr.x > 0)
	{
		MazePt next(curr.x - 1, curr.y);
		if (!m_visited[next.y * m_noGrid + next.x])
		{
			if (m_maze.Move(Maze::DIR_LEFT) == true)
			{
				m_myGrid[next.y * m_noGrid + next.x] = Maze::TILE_EMPTY;
				DFS(next);
				m_maze.Move(Maze::DIR_RIGHT);
			}
			else
			{
				m_myGrid[next.y * m_noGrid + next.x] = Maze::TILE_WALL;
			}
		}
	}
	//RIGHT
	if (curr.x < m_noGrid - 1)
	{
		MazePt next(curr.x + 1, curr.y);
		if (!m_visited[next.y * m_noGrid + next.x])
		{
			if (m_maze.Move(Maze::DIR_RIGHT) == true)
			{
				m_myGrid[next.y * m_noGrid + next.x] = Maze::TILE_EMPTY;
				DFS(next);
				m_maze.Move(Maze::DIR_LEFT);
			}
			else
			{
				m_myGrid[next.y * m_noGrid + next.x] = Maze::TILE_WALL;
			}
		}
	}
}

bool SceneTurn::BFS(MazePt start, MazePt end)
{
	std::fill(m_visited.begin(), m_visited.end(), false);
	while (!m_queue.empty())
		m_queue.pop();
	m_shortestPath.clear();
	m_queue.push(start);
	m_maze.SetNumMove(0);
	while (!m_queue.empty())
	{
		MazePt curr = m_queue.front();
		m_maze.SetCurr(curr);
		m_queue.pop();
		if (curr.x == end.x && curr.y == end.y)
		{
			while (!(curr.x == start.x && curr.y == start.y))
			{
				m_shortestPath.insert(m_shortestPath.begin(), curr);
				curr = m_previous[curr.y * m_noGrid + curr.x];
			}
			m_shortestPath.insert(m_shortestPath.begin(), curr);
			return true;
		}
		//UP
		if (curr.y < m_noGrid - 1)
		{
			MazePt next(curr.x, curr.y + 1);
			if (!m_visited[next.y * m_noGrid + next.x] && m_myGrid[next.y * m_noGrid + next.x] == Maze::TILE_EMPTY)
			{
				m_previous[next.y * m_noGrid + next.x] = curr;
				m_queue.push(next);
				m_visited[next.y * m_noGrid + next.x] = true;
			}
		}
		//DOWN
		if (curr.y > 0)
		{
			MazePt next(curr.x, curr.y - 1);
			if (!m_visited[next.y * m_noGrid + next.x] && m_myGrid[next.y * m_noGrid + next.x] == Maze::TILE_EMPTY)
			{
				m_previous[next.y * m_noGrid + next.x] = curr;
				m_queue.push(next);
				m_visited[next.y * m_noGrid + next.x] = true;
			}
		}
		//LEFT
		if (curr.x > 0)
		{
			MazePt next(curr.x - 1, curr.y);
			if (!m_visited[next.y * m_noGrid + next.x] && m_myGrid[next.y * m_noGrid + next.x] == Maze::TILE_EMPTY)
			{
				m_previous[next.y * m_noGrid + next.x] = curr;
				m_queue.push(next);
				m_visited[next.y * m_noGrid + next.x] = true;
			}
		}
		//RIGHT
		if (curr.x < m_noGrid - 1)
		{
			MazePt next(curr.x + 1, curr.y);
			if (!m_visited[next.y * m_noGrid + next.x] && m_myGrid[next.y * m_noGrid + next.x] == Maze::TILE_EMPTY)
			{
				m_previous[next.y * m_noGrid + next.x] = curr;
				m_queue.push(next);
				m_visited[next.y * m_noGrid + next.x] = true;
			}
		}
	}
	return false;
}

void SceneTurn::DFSOnce(GameObject * go)
{
	go->stack.push_back(go->curr);
	// Check Up
	if (!((go->curr.y + 1)*m_noGrid + go->curr.x) < 0)
	{
		if (!m_visited[(go->curr.y + 1)*m_noGrid + go->curr.x])
		{
			go->grid[(go->curr.y + 1)*m_noGrid + go->curr.x] = m_maze.See(((go->curr.y + 1)*m_noGrid + go->curr.x));
			if (m_maze.m_grid[(go->curr.y + 1)*m_noGrid + go->curr.x] == Maze::TILE_EMPTY)
			{
				go->curr = ((go->curr.y + 1)*m_noGrid + go->curr.x);
				return;
			}
		}
	}
	// Check Down
	else if (!((go->curr.y - 1)*m_noGrid + go->curr.x) < 0)
	{
		if (!m_visited[(go->curr.y - 1)*m_noGrid + go->curr.x])
		{
			go->grid[(go->curr.y - 1)*m_noGrid + go->curr.x] = m_maze.See(((go->curr.y - 1)*m_noGrid + go->curr.x));
			if (m_maze.m_grid[(go->curr.y - 1)*m_noGrid + go->curr.x] == Maze::TILE_EMPTY)
			{
				go->curr = ((go->curr.y - 1)*m_noGrid + go->curr.x);
				return;
			}
		}
	}
	// Check Left
	else if (!(go->curr.y * m_noGrid + (go->curr.x - 1)) < 0)
	{
		if (!m_visited[go->curr.y * m_noGrid + (go->curr.x - 1)])
		{
			go->grid[go->curr.y * m_noGrid + (go->curr.x - 1)] = m_maze.See((go->curr.y * m_noGrid + (go->curr.x - 1)));
			if (m_maze.m_grid[go->curr.y * m_noGrid + (go->curr.x - 1)] == Maze::TILE_EMPTY)
			{
				go->curr = (go->curr.y * m_noGrid + (go->curr.x - 1));
				return;
			}
		}
	}
	// Check Right
	else if (!(go->curr.y * m_noGrid + (go->curr.x + 1)) < 0)
	{
		if (!m_visited[go->curr.y * m_noGrid + (go->curr.x + 1)])
		{
			go->grid[go->curr.y * m_noGrid + (go->curr.x + 1)] = m_maze.See((go->curr.y * m_noGrid + (go->curr.x + 1)));
			if (m_maze.m_grid[go->curr.y * m_noGrid + (go->curr.x + 1)] == Maze::TILE_EMPTY)
			{
				go->curr = (go->curr.y * m_noGrid + (go->curr.x + 1));
				return;
			}
		}
	}
	go->stack.pop_back();
	if (!go->stack.empty())
	{
		return;
	}
}

bool SceneTurn::BFSLimit(GameObject * go, MazePt end, int limit)
{
	std::vector<MazePt> previous(m_noGrid*m_noGrid);
	std::vector<bool> visited(m_noGrid*m_noGrid, false);
	//std::fill(visited.begin(), visited.end(), false);
	std::queue<MazePt> queue;
	while (!queue.empty())
		queue.pop();
	go->path.clear();
	queue.push(go->curr);
	while (!queue.empty())
	{
		MazePt curr = queue.front();
		queue.pop();
		if (curr.x == end.x && curr.y == end.y)
		{
			while (!(curr.x == go->curr.x && curr.y == go->curr.y))
			{
				go->path.insert(go->path.begin(), curr);
				curr = m_previous[curr.y * m_noGrid + curr.x];
			}
			go->path.insert(go->path.begin(), curr);
			return true;
		}
		//UP
		if (curr.y < m_noGrid - 1)
		{
			MazePt next(curr.x, curr.y + 1);
			if (!visited[next.y * m_noGrid + next.x] && go->grid[next.y * m_noGrid + next.x] == Maze::TILE_EMPTY)
			{
				previous[next.y * m_noGrid + next.x] = curr;
				queue.push(next);
				visited[next.y * m_noGrid + next.x] = true;
			}
		}
		//DOWN
		if (curr.y > 0)
		{
			MazePt next(curr.x, curr.y + 1);
			if (!visited[next.y * m_noGrid + next.x] && go->grid[next.y * m_noGrid + next.x] == Maze::TILE_EMPTY)
			{
				previous[next.y * m_noGrid + next.x] = curr;
				queue.push(next);
				visited[next.y * m_noGrid + next.x] = true;
			}
		}
		//LEFT
		if (curr.x > 0)
		{
			MazePt next(curr.x, curr.y + 1);
			if (!visited[next.y * m_noGrid + next.x] && go->grid[next.y * m_noGrid + next.x] == Maze::TILE_EMPTY)
			{
				previous[next.y * m_noGrid + next.x] = curr;
				queue.push(next);
				visited[next.y * m_noGrid + next.x] = true;
			}
		}
		//RIGHT
		if (curr.x < m_noGrid - 1)
		{
			MazePt next(curr.x, curr.y + 1);
			if (!visited[next.y * m_noGrid + next.x] && go->grid[next.y * m_noGrid + next.x] == Maze::TILE_EMPTY)
			{
				previous[next.y * m_noGrid + next.x] = curr;
				queue.push(next);
				visited[next.y * m_noGrid + next.x] = true;
			}
		}
	}
	return false;
}

void SceneTurn::Update(double dt)
{
	SceneBase::Update(dt);

	m_timer += (float)(m_speed * dt);
	if (m_timer > TURN_TIME)
	{
		++m_turn;
		m_timer = 0;
		for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
		{
			GameObject *go = (GameObject*)*it;
			DFSOnce(go);
		}
	}

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
		//Exercise: Implement Reset button
	}

	// Movement
	static bool b_WButtonState = false;
	if (!b_WButtonState && Application::IsKeyPressed('W'))
	{
		b_WButtonState = true;
		std::cout << "W_BUTTON DOWN" << std::endl;
		
	}
	else if (b_WButtonState && !Application::IsKeyPressed('W'))
	{
		b_WButtonState = false;
		std::cout << "W_BUTTON UP" << std::endl;
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
		if (posX < m_noGrid * m_gridSize && posY < m_noGrid * m_gridSize)
		{
			m_end.Set((int)(posX / m_gridSize), (int)(posY / m_gridSize));
			BFS(m_start, m_end);
			/*std::cout << "Path:";
			for (auto tile : m_shortestPath)
			{
				std::cout << "(" << tile.x << "," << tile.y << ")";
			}
			std::cout << std::endl;*/
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

		int x_here, y_here;
		GameObject *go = FetchGO(GameObject::GO_NPC);
		go->grid.resize(m_noGrid * m_noGrid);
		go->visited.resize(m_noGrid * m_noGrid);
		std::fill(go->grid.begin(), go->grid.end(), Maze::TILE_FOG);
		std::fill(go->visited.begin(), go->visited.end(), false);
		do {
			x_here = Math::RandIntMinMax(0, m_noGrid - 1);
			y_here = Math::RandIntMinMax(0, m_noGrid - 1);
		} while (m_maze.m_grid[go->curr.y * m_noGrid + go->curr.x] != Maze::TILE_EMPTY);
		go->curr.x = x_here;
		go->curr.y = y_here;
		go->stack.push_back(go->curr);
		go->scale.x = m_gridSize;
		go->scale.y = m_gridSize;
		go->scale.z = m_gridSize;
		go->grid[go->curr.y * m_noGrid + go->curr.x] = Maze::TILE_EMPTY;
		std::cout << "Ding" << std::endl;
	}
	else if (bSpaceState && !Application::IsKeyPressed(VK_SPACE))
	{
		bSpaceState = false;
	}
}


void SceneTurn::RenderGO(GameObject *go)
{
	switch (go->type)
	{
	case GameObject::GO_NONE: //Render GO_NPC
		break;
	case GameObject::GO_NPC:
		modelStack.PushMatrix();
		modelStack.Translate(m_gridOffset + m_gridSize * go->curr.x, m_gridOffset + m_gridSize * go->curr.y, 1);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_MINER], false);
		modelStack.PopMatrix();
		break;
	}
}

void SceneTurn::Render()
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

	//Render tiles 
	//for (int row = 0; row < m_noGrid; ++row)
	//{
	//	for (int col = 0; col < m_noGrid; ++col)
	//	{
	//		modelStack.PushMatrix();
	//		modelStack.Translate(m_gridOffset + m_gridSize * col, m_gridOffset + m_gridSize * row, 0);
	//		modelStack.Scale(m_gridSize, m_gridSize, m_gridSize);
	//		switch (m_myGrid[row * m_noGrid + col])
	//		{
	//		case Maze::TILE_WALL:
	//			RenderMesh(meshList[GEO_GREYQUAD], false);
	//			break;
	//		case Maze::TILE_FOG:
	//			RenderMesh(meshList[GEO_BLACKQUAD], false);
	//			break;
	//		case Maze::TILE_EMPTY:
	//			RenderMesh(meshList[GEO_WHITEQUAD], false);
	//			break;
	//		}
	//		modelStack.PopMatrix();
	//	}
	//}
	for (int row = 0; row < m_noGrid; ++row)
	{
		for (int col = 0; col < m_noGrid; ++col)
		{
			modelStack.PushMatrix();
			modelStack.Translate(m_gridOffset + m_gridSize * col, m_gridOffset + m_gridSize * row, 0);
			modelStack.Scale(m_gridSize, m_gridSize, m_gridSize);
			switch (m_maze.m_grid[row * m_noGrid + col])
			{
			case Maze::TILE_WALL:
				RenderMesh(meshList[GEO_BEDROCK], false);
				break;
			case Maze::TILE_FOG:
				RenderMesh(meshList[GEO_BLACKQUAD], false);
				break;
			case Maze::TILE_EMPTY:
				RenderMesh(meshList[GEO_DIRT], false);
				break;
			case Maze::TILE_POINT:
				RenderMesh(meshList[GEO_DIRTDIAMOND], false);
				break;
			}
			modelStack.PopMatrix();
		}
	}
	//Render curr point
	/*MazePt curr = m_maze.GetCurr();
	modelStack.PushMatrix();
	modelStack.Translate(m_gridOffset + m_gridSize * curr.x, m_gridOffset + m_gridSize * curr.y, 1);
	modelStack.Scale(m_gridSize, m_gridSize, m_gridSize);
	RenderMesh(meshList[GEO_QUEEN], false);
	modelStack.PopMatrix();*/
	//Render shortest path
	/*for (auto tile : m_shortestPath)
	{
		modelStack.PushMatrix();
		modelStack.Translate(m_gridOffset + m_gridSize * tile.x, m_gridOffset + m_gridSize * tile.y, 1);
		modelStack.Scale(m_gridSize, m_gridSize, m_gridSize);
		RenderMesh(meshList[GEO_QUEEN], false);
		modelStack.PopMatrix();
	}*/

	//Render m_goList
	/*for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (go->active)
		{
			RenderGO(go);
		}
	}*/
	for (auto go : m_goList)
	{
		RenderGO(go);
	}

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
	ss << "Turn:" << m_turn;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 50, 9);

	ss.str("");
	ss << "Turn Time:" << m_timer;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 50, 12);

	ss.str("");
	ss << "Turn Maze " << m_mazeKey;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 50, 0);
}

void SceneTurn::Exit()
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
