#include "SceneMovement.h"
#include "GL\glew.h"
#include "Application.h"
#include <sstream>

#include "StateFish.h"
#include "StateShark.h"
#include "StateDefault.h"
#include "StatePrisoners.h"

SceneMovement::SceneMovement()
{
}

SceneMovement::~SceneMovement()
{
}

void SceneMovement::Init()
{
	SceneBase::Init();

	//Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	//Physics code here
	m_speed = 1.f;

	Math::InitRNG();
	SceneData::GetInstance()->SetObjectCount(0);
	SceneData::GetInstance()->SetFishCount(0);
	SceneData::GetInstance()->SetSharkKillCount(0);
	SceneData::GetInstance()->SetnoGrid(20);
	SceneData::GetInstance()->SetGridSize(m_worldHeight / SceneData::GetInstance()->GetnoGrid());
	SceneData::GetInstance()->SetGridOffset(SceneData::GetInstance()->GetGridSize() / 2);
	//m_hourOfTheDay = 0;
	PostOffice::GetInstance()->Register("SceneMovement", dynamic_cast<ObjectBase*>(this));

	SceneData::GetInstance()->SetRoleCallPos(Vector3(SceneData::GetInstance()->GetGridOffset() + 13 * SceneData::GetInstance()->GetGridSize(), SceneData::GetInstance()->GetGridOffset() + 7 * SceneData::GetInstance()->GetGridSize(), 0));
	SceneData::GetInstance()->SetClockTime(0);
	prisoner_count = 0;

	while (prisoner_count <= 6)
	{
		int randomX = Math::RandIntMinMax(0, SceneData::GetInstance()->GetnoGrid() - 1);
		int randomY = Math::RandIntMinMax(0, SceneData::GetInstance()->GetnoGrid() - 1);
		GameObject *go = FetchGO(GameObject::GO_PRISONERS);
		go->id = SceneData::GetInstance()->GetFishCount();
		go->type = GameObject::GO_PRISONERS;
		go->scale.Set(SceneData::GetInstance()->GetGridSize(), SceneData::GetInstance()->GetGridSize(), SceneData::GetInstance()->GetGridSize());
		go->pos.Set(SceneData::GetInstance()->GetGridOffset() + randomX * SceneData::GetInstance()->GetGridSize(), SceneData::GetInstance()->GetGridOffset() + randomY * SceneData::GetInstance()->GetGridSize(), 0);
		go->target = go->pos;
		go->bed_location.Set(SceneData::GetInstance()->GetGridOffset() + 6 * SceneData::GetInstance()->GetGridSize(), SceneData::GetInstance()->GetGridOffset() + 12 * SceneData::GetInstance()->GetGridSize(), 0);
		go->movespeed = 5.f;
		go->sm->SetNextState("StatePrisoners_Sleep");
		++prisoner_count;
	}
}

GameObject* SceneMovement::FetchGO(GameObject::GAMEOBJECT_TYPE type)
{
	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (!go->active && type == go->type)
		{
			go->active = true;
			SceneData::GetInstance()->SetObjectCount(SceneData::GetInstance()->GetObjectCount() + 1);
			return go;
		}
	}
	for (unsigned i = 0; i < 5; ++i)
	{
		//GameObject *go = new GameObject(GameObject::GO_BALL);
		GameObject *go = new GameObject(type);
		m_goList.push_back(go);
		if (go->type == GameObject::GO_FISH)
		{
			go->sm = new StateMachine();
			go->sm->AddState(new StateTooFull("StateTooFull", go));
			go->sm->AddState(new StateFull("StateFull", go));
			go->sm->AddState(new StateHungry("StateHungry", go));
			go->sm->AddState(new StateDead("StateDead", go));
		}
		else if (go->type == GameObject::GO_SHARK)
		{
			go->sm = new StateMachine();
			go->sm->AddState(new StateCrazy("StateCrazy", go));
			go->sm->AddState(new StateFull("StateNaughty", go));
			go->sm->AddState(new StateHungry("StateHappy", go));
		}
		else if (go->type == GameObject::GO_PRISONERS)
		{
			go->sm = new StateMachine();
			go->sm->AddState(new StatePrisoners_Sleep("StatePrisoners_Sleep", go));
			go->sm->AddState(new StatePrisoners_RoleCall("StatePrisoners_RoleCall", go));
		}
		else
		{
			go->sm = new StateMachine();
			go->sm->AddState(new StateDefault("StateDefault", go));
		}
			
	}
	return FetchGO(type);
}

void SceneMovement::Update(double dt)
{
	SceneBase::Update(dt);

	static const float BALL_SPEED = 5.f;
	static const float HOUR_SPEED = 1.f;

	float m_clocktime = SceneData::GetInstance()->GetClockTime();

	// Update time
	if (m_clocktime >= 2400)
		m_clocktime = 0;
	else
		m_clocktime += static_cast<float>(dt * m_speed * 25);

	SceneData::GetInstance()->SetClockTime(m_clocktime);

	//Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	if(Application::IsKeyPressed(VK_OEM_MINUS))
	{
		m_speed = Math::Max(0.f, m_speed - 0.1f);
	}
	if(Application::IsKeyPressed(VK_OEM_PLUS))
	{
		m_speed += 0.1f;
	}

	//m_hourOfTheDay += HOUR_SPEED * static_cast<float>(dt) * m_speed;
	/*if (m_hourOfTheDay >= 24.f)
		m_hourOfTheDay = 0;*/

	//Input Section
	static bool bLButtonState = false;
	if(!bLButtonState && Application::IsMousePressed(0))
	{
		bLButtonState = true;
		std::cout << "LBUTTON DOWN" << std::endl;
	}
	else if(bLButtonState && !Application::IsMousePressed(0))
	{
		bLButtonState = false;
		std::cout << "LBUTTON UP" << std::endl;
	}
	static bool bRButtonState = false;
	if(!bRButtonState && Application::IsMousePressed(1))
	{
		bRButtonState = true;
		std::cout << "RBUTTON DOWN" << std::endl;
	}
	else if(bRButtonState && !Application::IsMousePressed(1))
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
	static bool bPState = false;
	if (!bPState && Application::IsKeyPressed('P') && prisoner_count <= 8)
	{
		bPState = true;

		int randomX = Math::RandIntMinMax(0, SceneData::GetInstance()->GetnoGrid() - 1);
		int randomY = Math::RandIntMinMax(0, SceneData::GetInstance()->GetnoGrid() - 1);
		GameObject *go = FetchGO(GameObject::GO_PRISONERS);
		go->id = SceneData::GetInstance()->GetFishCount();
		go->type = GameObject::GO_PRISONERS;
		go->scale.Set(SceneData::GetInstance()->GetGridSize(), SceneData::GetInstance()->GetGridSize(), SceneData::GetInstance()->GetGridSize());
		go->pos.Set(SceneData::GetInstance()->GetGridOffset() + randomX * SceneData::GetInstance()->GetGridSize(), SceneData::GetInstance()->GetGridOffset() + randomY * SceneData::GetInstance()->GetGridSize(), 0);
		go->target = go->pos;
		go->bed_location.Set(SceneData::GetInstance()->GetGridOffset() + 6 * SceneData::GetInstance()->GetGridSize(), SceneData::GetInstance()->GetGridOffset() + 12 * SceneData::GetInstance()->GetGridSize(), 0);
		go->movespeed = 5.f;
		go->sm->SetNextState("StatePrisoners_Sleep");
		++prisoner_count;
	}
	else if (bPState && !Application::IsKeyPressed('P'))
	{
		bPState = false;
	}

	// Movement & Collision Section
	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject*)*it;
		if (!go->active)
			continue;

		// Food normal movement
		if (go->type == GameObject::GO_FOOD)
		{
			Vector3 dir = go->target - go->pos;
			if (dir.Length() < dt * m_speed * go->movespeed)
			{
				//GO->pos reach target
				go->pos = go->target;
				float random = Math::RandFloatMinMax(0.f, 1.f);
				//Exercise: use probability to decide go up or right
				//Exercise: change the conditions so that the game objects can move randomly
				// Random movement
				if (random > 0.5f)
				{
					if (random > 0.75f) // Down
						go->target.y -= SceneData::GetInstance()->GetGridSize();
					else  // Right
						go->target.x += SceneData::GetInstance()->GetGridSize();
				}
				else if (random < 0.5f)
				{
					if (random < 0.25f) // Left
						go->target.x -= SceneData::GetInstance()->GetGridSize();
					else // Up
						go->target.y += SceneData::GetInstance()->GetGridSize();
				}
			}
			else
			{
				dir.Normalize();
				go->pos += dir * static_cast<float>(dt) * m_speed * go->movespeed;
			}	
		}
		else
		{
			go->sm->Update(dt * m_speed);
		}
		// Collision
		if (go->type == GameObject::GO_FISH)
		{
			for (std::vector<GameObject *>::iterator it2 = m_goList.begin(); it2 != m_goList.end(); ++it2)
			{
				GameObject *go2 = (GameObject *)*it2;
				if (go2->active)
				{
					if (go2->type == GameObject::GO_FOOD)
					{
						Vector3 distance = go2->pos - go->pos;
						float radius = SceneData::GetInstance()->GetGridOffset() * 2; // Incrase hit box
						if (distance.LengthSquared() <= radius * radius)
						{
							go2->active = false;
							go->energy += 5;

						}
					}
					if (go2->type == GameObject::GO_SHARK) // Shark collsion check with fish
					{
						// Eating
						Vector3 distance = go2->pos - go->pos;
						float radius = SceneData::GetInstance()->GetGridOffset() * 2; // Incrase hitbox
						if (distance.LengthSquared() <= radius * radius && go->sm->GetCurrentState() != "StateDead")
						{
							go->sm->SetNextState("StateDead");
							//go1->currState == GameObject::FISH_STATE::STATE_DEAD;
							SceneData::GetInstance()->SetSharkKillCount(SceneData::GetInstance()->GetSharkKillCount() + 1);
						}
					}
				}
			}

		}
	
		//Exercise: set boundaries so that game objects would not leave scene
		if (go->target.x >= SceneData::GetInstance()->GetGridSize() * SceneData::GetInstance()->GetnoGrid())
		{
			go->target.x = SceneData::GetInstance()->GetGridSize() *  SceneData::GetInstance()->GetnoGrid() / 2;
		}
		else if (go->target.x <= SceneData::GetInstance()->GetGridSize())
		{
			go->target.x = SceneData::GetInstance()->GetGridSize() / 2;
		}
		if (go->target.y >= SceneData::GetInstance()->GetGridSize()  * SceneData::GetInstance()->GetnoGrid())
		{
			go->target.y = SceneData::GetInstance()->GetGridSize() *  SceneData::GetInstance()->GetnoGrid() / 2;
		}
		else if (go->target.y <= SceneData::GetInstance()->GetGridSize())
		{
			go->target.y = SceneData::GetInstance()->GetGridSize() / 2;
		}
	}
}


void SceneMovement::RenderGO(GameObject *go)
{
	std::ostringstream ss;
	switch(go->type)
	{
	case GameObject::GO_BALL:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_BALL], false);
	
		ss << go->id;
		RenderText(meshList[GEO_TEXT], ss.str(), Color(0, 0, 0));
		modelStack.PopMatrix();
		break;
	case GameObject::GO_FISH:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_FISH], false);

		ss.precision(2);
		ss << go->energy;
		RenderText(meshList[GEO_TEXT], ss.str(), Color(0, 0, 0));
		modelStack.PopMatrix();
		break;
	case GameObject::GO_SHARK:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_SHARK], false);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_FOOD:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_FOOD], false);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_PRISONERS:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_PRISONERS], false);
		modelStack.PopMatrix();
		break;
	}
}

bool SceneMovement::Handle(Message * msg)
{
	MessageWRU *messageWRU = dynamic_cast<MessageWRU*>(msg);
	if (messageWRU)
	{
		if (messageWRU->go->type == GameObject::GO_FISH)
		{
			messageWRU->go->nearest = NULL;
			float nearestDistance = messageWRU->threshold;
			// Process
			switch (messageWRU->type)
			{
			case MessageWRU::NEAREST_SHARK:
			{
				for (std::vector<GameObject*>::iterator it2 = m_goList.begin(); it2 != m_goList.end(); ++it2)
				{
					GameObject *go2 = (GameObject*)*it2;
					if (go2->active && go2->type == GameObject::GO_SHARK)
					{
						float distance = (messageWRU->go->pos - go2->pos).Length();
						if (distance < nearestDistance)
						{
							nearestDistance = distance;
							messageWRU->go->nearest = go2;
						}
					}
					else
						continue;
				}
				break;
			}
			case MessageWRU::NEAREST_FISHFOOD:
			{
				for (std::vector<GameObject*>::iterator it2 = m_goList.begin(); it2 != m_goList.end(); ++it2)
				{
					GameObject *go2 = (GameObject*)*it2;
					if (go2->active && go2->type == GameObject::GO_FOOD)
					{
						float distance = (messageWRU->go->pos - go2->pos).Length();
						if (distance < nearestDistance)
						{
							nearestDistance = distance;
							messageWRU->go->nearest = go2;
						}
					}
					else
						continue;
				}
				break;
			}
			default:
				break;
			}
		}
		else if (messageWRU->go->type == GameObject::GO_SHARK)
		{
			messageWRU->go->nearest = NULL;
			
			switch (messageWRU->type)
			{
			case MessageWRU::NEAREST_FULLFISH:
			{
				float nearestDistance = messageWRU->threshold * messageWRU->threshold;
				for (std::vector<GameObject*>::iterator it2 = m_goList.begin(); it2 != m_goList.end(); ++it2)
				{
					GameObject *go2 = (GameObject*)*it2;
					if (go2->active && go2->type == GameObject::GO_FISH && go2->sm->GetCurrentState() == "StateFull" || go2->sm->GetCurrentState() == "StateTooFull")
					{
						float distance = (messageWRU->go->pos - go2->pos).Length();
						if (distance < nearestDistance)
						{
							nearestDistance = distance;
							messageWRU->go->nearest = go2;
						}
					}
					else
						continue;
				}
				break;
			}
			case MessageWRU::HIGHEST_ENERGYFISH:
			{
				float maxEnergy = 0.f;
				for (std::vector<GameObject*>::iterator it2 = m_goList.begin(); it2 != m_goList.end(); ++it2)
				{
					GameObject *go2 = (GameObject*)*it2;
					if (go2->active && go2->type == GameObject::GO_FISH && go2->sm->GetCurrentState() == "StateDead")
					{
						if (go2->energy > maxEnergy)
						{
							maxEnergy = go2->energy;
							messageWRU->go->nearest = go2;
						}
					}
					else
						continue;
				}
				break;
	
			}
			}
		}
		delete msg;
		return true;
	}
	if (msg != NULL)
		delete msg;

	return false;
}

void SceneMovement::Render()
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
	RenderMesh(meshList[GEO_BG], false);
	modelStack.PopMatrix();

	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (go->active)
		{
			RenderGO(go);
		}
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
	ss << "Amount of enities:" << SceneData::GetInstance()->GetObjectCount();
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 50, 9);

	ss.str("");
	ss.precision(4);
	ss << "Time = " << SceneData::GetInstance()->GetClockTime();
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 50, 12);

	/*ss.str("");
	ss.precision(4);
	ss << "SHARK KILLS:" << SceneData::GetInstance()->GetSharkKillCount();
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 50, 15);*/
	
	RenderTextOnScreen(meshList[GEO_TEXT], "Movement", Color(0, 1, 0), 3, 50, 0);
}

void SceneMovement::Exit()
{
	SceneBase::Exit();
	//Cleanup GameObjects
	while(m_goList.size() > 0)
	{
		GameObject *go = m_goList.back();
		delete go;
		m_goList.pop_back();
	}
	if(m_ghost)
	{
		delete m_ghost;
		m_ghost = NULL;
	}
}
