#include "SceneReversi.h"
#include "GL\glew.h"
#include "Application.h"
#include <sstream>

SceneReversi::SceneReversi()
{
}

SceneReversi::~SceneReversi()
{
}

void SceneReversi::Init()
{
	SceneBase::Init();

	//Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	//Physics code here
	m_speed = 1.f;

	Math::InitRNG();

	m_noGrid = 8;
	m_gridSize = m_worldHeight / m_noGrid;
	m_gridOffset = m_gridSize / 2;

	Reset();
}

GameObject* SceneReversi::FetchGO()
{
	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (!go->active)
		{
			go->active = true;
			++m_objectCount;
			return go;
		}
	}
	for (unsigned i = 0; i < 10; ++i)
	{
		GameObject *go = new GameObject(GameObject::GO_CROSS);
		m_goList.push_back(go);
	}
	return FetchGO();
}

//Exercise: This function should return 0 to N, i.e. AI player's decision to put its disk on one of the grids
int SceneReversi::GetAIDecision()
{
	return 0;
}

void SceneReversi::Reset()
{
	m_objectCount = 0;
	m_bBlackTurn = true;
	m_bGameOver = false;
	m_winner = WHO_CONTENT::WHO_NONE;
	for (int i = 0; i < m_noGrid * m_noGrid; ++i)
	{
		m_grid[i] = WHO_NONE;
	}
	int half = m_noGrid / 2 - 1;
	m_grid[half * m_noGrid + half] = WHO_BLACK;
	m_grid[half * m_noGrid + half + 1] = WHO_WHITE;
	m_grid[(half + 1) * m_noGrid + half] = WHO_WHITE;
	m_grid[(half + 1) * m_noGrid + half + 1] = WHO_BLACK;

}

bool SceneReversi::IsInBoard(int posX, int posY)
{
	if (posX < 0)
		return false;
	if (posY < 0)
		return false;
	if (posX > m_noGrid)
		return false;
	if (posY > m_noGrid)
		return false;

	return true;
}

int SceneReversi::ConvertXYPos(int posX, int posY)
{

	return posX + posY * m_noGrid;
}

void SceneReversi::ConvertArrayPos(int pos, int & posX, int & posY)
{
	posX = pos % m_noGrid;
	posY = pos / m_noGrid;
}

bool SceneReversi::IsLegalMove(int posX, int posY, bool flip)
{
	int result = 0;
	if (CheckSlot(posX - 1, posY, -1, 0, 0, flip))
		++result;
	if (CheckSlot(posX + 1, posY, 1, 0, 0, flip))
		++result;
	if (CheckSlot(posX, posY - 1, 0, -1, 0, flip))
		++result;
	if (CheckSlot(posX, posY + 1, 0, 1, 0, flip))
		++result;
	if (CheckSlot(posX - 1, posY - 1, -1, -1, 0, flip))
		++result;
	if (CheckSlot(posX + 1, posY + 1, 1, 1, 0, flip))
		++result;
	if (CheckSlot(posX - 1, posY + 1, -1, 1, 0, flip))
		++result;
	if (CheckSlot(posX + 1, posY - 1, 1, -1, 0, flip))
		++result;
	return result > 0;
}

bool SceneReversi::CheckSlot(int posX, int posY, int dirX, int dirY, int count, bool flip)
{
	// Terminating Clauses

	// Not in board
	if (!IsInBoard(posX, posY))
		return false;

	// Check what search value
	WHO_CONTENT searchValue;
	if (m_bBlackTurn)
		searchValue = WHO_BLACK;
	else
		searchValue = WHO_WHITE;

	bool result = false;

	// Check if blank slot
	if (m_grid[ConvertXYPos(posX, posY)] == WHO_NONE)
		return false;

	else if (m_grid[ConvertXYPos(posX, posY)] == searchValue)
	{
		// Found the searched value
		if (count == 0)
			return false;
		else
			return true;
	}

	else
	{
		// This is for all opposing color titles
		// Process & Recursion
		result = CheckSlot(posX + dirX, posY + dirY, dirX, dirY, count + 1, flip);

		// Recusion stop will stop here
		//
		if (result && flip)
		{
			m_grid[ConvertXYPos(posX, posY)] = searchValue;
		}
	}
	return false;
}

int SceneReversi::GetBoardValue()
{
	/*int result = 0;
	int total = m_noGrid * m_noGrid;
	for (int i = 0; i < total; ++i)
	{
		int x, y;
		ConvertXYPos(x, y);
	}
	*/
	return 0;
}

void SceneReversi::Update(double dt)
{
	SceneBase::Update(dt);

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
	if (Application::IsKeyPressed('R'))
	{
		Reset();
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
		if (posX > 0 && posX < m_noGrid * m_gridSize && posY > 0 && posY < m_noGrid * m_gridSize)
		{
			int gridX = static_cast<int>(posX / m_gridSize);
			int gridY = static_cast<int>(posY / m_gridSize);
			int index = gridY * m_noGrid + gridX;
			std::cout << index << std::endl;
			if (m_grid[index] == WHO_CONTENT::WHO_NONE && IsLegalMove(gridX,gridY))
			{
				// Fliper
				IsLegalMove(gridX, gridY, true);
				if (m_bBlackTurn)
				{
					//Exercise: Check if this input is a valid move
					m_grid[index] = WHO_CONTENT::WHO_BLACK;
					m_bBlackTurn = false;
				}
				else //Exercise: For AI to take over
				{
					//Exercise: Check if this input is a valid move
					m_grid[index] = WHO_CONTENT::WHO_WHITE;
					m_bBlackTurn = true;
				}
				//Game Logic Section - call methods
				//Exercise: Flips the existing disks on board
				//Exercise: Check if there is valid move for current color, if not forfeit the turn 
				//Exercise: Check draw, black wins or white wins
			}
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

}


void SceneReversi::RenderGO(GameObject *go)
{
}

void SceneReversi::Render()
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
	RenderMesh(meshList[GEO_REVERSI], false);
	modelStack.PopMatrix();

	for (int i = 0; i < m_noGrid * m_noGrid; ++i)
	{
		if (m_grid[i] == WHO_BLACK)
		{
			int gridX = i % m_noGrid;
			int gridY = i / m_noGrid;
			modelStack.PushMatrix();
			modelStack.Translate(m_gridOffset + gridX * m_gridSize, m_gridOffset + gridY * m_gridSize, 0);
			modelStack.Scale(m_gridSize, m_gridSize, 1);
			RenderMesh(meshList[GEO_BLACK], false);
			modelStack.PopMatrix();
		}
		else if (m_grid[i] == WHO_WHITE)
		{
			int gridX = i % m_noGrid;
			int gridY = i / m_noGrid;
			modelStack.PushMatrix();
			modelStack.Translate(m_gridOffset + gridX * m_gridSize, m_gridOffset + gridY * m_gridSize, 0);
			modelStack.Scale(m_gridSize, m_gridSize, 1);
			RenderMesh(meshList[GEO_WHITE], false);
			modelStack.PopMatrix();
		}
	}

	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (go->active)
		{
			RenderGO(go);
		}
	}

	//On screen text
	static std::string winnerText[] = { "Draw", "Black wins", "White wins" };

	std::ostringstream ss;
	ss.precision(3);
	ss << "Speed:" << m_speed;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 50, 6);

	ss.str("");
	ss.precision(5);
	ss << "FPS:" << fps;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 50, 3);

	ss.str("");
	ss << "Count:" << m_objectCount;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 50, 9);

	ss.str("");
	ss << "Turn:" << (m_bBlackTurn ? "Black" : "White");
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 50, 12);

	if (m_bGameOver)
	{
		ss.str("");
		ss << winnerText[static_cast<int>(m_winner)];
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 50, 15);
	}

	RenderTextOnScreen(meshList[GEO_TEXT], "Reversi (R to reset)", Color(0, 1, 0), 3, 50, 0);
}

void SceneReversi::Exit()
{
	SceneBase::Exit();
	//Cleanup GameObjects
	while (m_goList.size() > 0)
	{
		GameObject *go = m_goList.back();
		delete go;
		m_goList.pop_back();
	}
	if (m_ghost)
	{
		delete m_ghost;
		m_ghost = NULL;
	}
}
