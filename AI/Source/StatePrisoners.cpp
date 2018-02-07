#include "StatePrisoners.h"

// Prisoners go to sleep
StatePrisoners_Sleep::StatePrisoners_Sleep(const std::string & stateID, GameObject * go)
	: State(stateID),
	m_go(go)
{
}
StatePrisoners_Sleep::~StatePrisoners_Sleep()
{
}
void StatePrisoners_Sleep::Enter()
{
	m_go->target = m_go->pos;
	m_go->movespeed = 10.f;
}
void StatePrisoners_Sleep::Update(double dt)
{
	// Check time of day
	if (SceneData::GetInstance()->GetClockTime() >= 500 && SceneData::GetInstance()->GetClockTime() < 600)
		m_go->sm->SetNextState("StatePrisoners_RoleCall");


	// Move towards their bed location in their cell
	if (m_go->pos != m_go->bed_location)
	{
		Vector3 dir = m_go->target - m_go->pos;
		float m_gridSize = SceneData::GetInstance()->GetGridSize();
		if (dir.Length() < dt * m_go->movespeed)
		{
			//GO->pos reach target
			m_go->pos = m_go->target;
			float random = Math::RandFloatMinMax(0.f, 1.f);
			//Exercise: use probability to decide go up or right
			//Exercise: change the conditions so that the game objects can move randomly
			// Random movement
			if (random > 0.75f && m_go->pos.x < m_go->bed_location.x)
				m_go->target.x += m_gridSize;
			else if (random > 0.5f && m_go->pos.x > m_go->bed_location.x)
				m_go->target.x -= m_gridSize;
			else if(random > 0.25f && m_go->pos.y < m_go->bed_location.y)	
					m_go->target.y += m_gridSize;
			else if (random > 0.f && m_go->pos.y > m_go->bed_location.y)
					m_go->target.y -= m_gridSize;			
		}
		else
		{
			dir.Normalize();
			m_go->pos += dir * static_cast<float>(dt) *  m_go->movespeed;
		}
	}	
}
void StatePrisoners_Sleep::Exit()
{
}

// Prisoners Start their day and end their day
StatePrisoners_RoleCall::StatePrisoners_RoleCall(const std::string & stateID, GameObject * go)
	: State(stateID),
	m_go(go)
{
}
StatePrisoners_RoleCall::~StatePrisoners_RoleCall()
{
}
void StatePrisoners_RoleCall::Enter()
{
	m_go->target = m_go->pos;
	m_go->movespeed = 10.f;
}
void StatePrisoners_RoleCall::Update(double dt)
{
	// Check time of day
	/*if (SceneData::GetInstance()->GetClockTime() >= 600)
		m_go->sm->SetNextState("StatePrisoners_Eat");*/
	if(SceneData::GetInstance()->GetClockTime() >= 2200 || SceneData::GetInstance()->GetClockTime() < 500)
		m_go->sm->SetNextState("StatePrisoners_Sleep");

	// Variables
	Vector3 m_rollcallPos = SceneData::GetInstance()->GetRoleCallPos();

	// Move towards roll call location
	if (m_go->pos != m_rollcallPos)
	{
		Vector3 dir = m_go->target - m_go->pos;
		float m_gridSize = SceneData::GetInstance()->GetGridSize();
		if (dir.Length() < dt * m_go->movespeed)
		{
			//GO->pos reach target
			m_go->pos = m_go->target;
			float random = Math::RandFloatMinMax(0.f, 1.f);
			//Exercise: use probability to decide go up or right
			//Exercise: change the conditions so that the game objects can move randomly
			// Random movement
			if (random > 0.75f && m_go->pos.x < m_rollcallPos.x)
				m_go->target.x += m_gridSize;
			else if (random > 0.5f && m_go->pos.x > m_rollcallPos.x)
				m_go->target.x -= m_gridSize;
			else if (random > 0.25f && m_go->pos.y < m_rollcallPos.y)
				m_go->target.y += m_gridSize;
			else if (random > 0.f && m_go->pos.y > m_rollcallPos.y)
				m_go->target.y -= m_gridSize;
		}
		else
		{
			dir.Normalize();
			m_go->pos += dir * static_cast<float>(dt) *  m_go->movespeed;
		}
	}
}
void StatePrisoners_RoleCall::Exit()
{
}

// Prisoners Randomly chooses to escape

StatePrisoners_Escape::StatePrisoners_Escape(const std::string & stateID, GameObject * go)
	: State(stateID),
	m_go(go)
{
}

StatePrisoners_Escape::~StatePrisoners_Escape()
{
}

void StatePrisoners_Escape::Enter()
{
}

void StatePrisoners_Escape::Update(double dt)
{
}

void StatePrisoners_Escape::Exit()
{
}
