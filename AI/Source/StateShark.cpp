#include "StateShark.h"

#include "PostOffice.h"
#include "ConcreteMessages.h"

StateCrazy::StateCrazy(const std::string & stateID, GameObject * go)
	:State(stateID),
	m_go(go)
{
}

StateCrazy::~StateCrazy()
{
}

void StateCrazy::Enter()
{
	m_go->movespeed = 20.f;
}

void StateCrazy::Update(double dt)
{
	// Check fish count states
	if (SceneData::GetInstance()->GetFishCount() <= 10)
		m_go->sm->SetNextState("StateNaughty");

	// Search for highest energy fish
	if(m_go->nearest == NULL)
		PostOffice::GetInstance()->Send("SceneMovement", new MessageWRU(m_go, MessageWRU::HIGHEST_ENERGYFISH, 0.f));
	if (m_go->nearest)
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
			if (random > 0.5f)
			{
				if (m_go->pos.x < m_go->nearest->pos.x) // Right
					m_go->target.x += m_gridSize;
				else  // Left
					m_go->target.x -= m_gridSize;
			}
			else
			{
				if (m_go->pos.x < m_go->nearest->pos.y) // Up
					m_go->target.y += m_gridSize;
				else // Down
					m_go->target.y -= m_gridSize;
			}
		}
		else
		{
			dir.Normalize();
			m_go->pos += dir * static_cast<float>(dt) *  m_go->movespeed;
		}
	}

}

void StateCrazy::Exit()
{
}

StateNaughty::StateNaughty(const std::string & stateID, GameObject * go)
	:State(stateID),
	m_go(go)
{
}

StateNaughty::~StateNaughty()
{
}

void StateNaughty::Enter()
{
	m_go->movespeed = 15.f;
}

void StateNaughty::Update(double dt)
{
	// Check fish count states
	if (SceneData::GetInstance()->GetFishCount() > 10)
		m_go->sm->SetNextState("StateCrazy");
	else if (SceneData::GetInstance()->GetFishCount() < 5)
		m_go->sm->SetNextState("StateHappy");

	// Search for nearest fish
	if (m_go->nearest == NULL)
		PostOffice::GetInstance()->Send("SceneMovement", new MessageWRU(m_go, MessageWRU::NEAREST_FULLFISH, 0.f));
	if (m_go->nearest)
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
			if (random > 0.5f)
			{
				if (m_go->pos.x < m_go->nearest->pos.x) // Right
					m_go->target.x += m_gridSize;
				else  // Left
					m_go->target.x -= m_gridSize;
			}
			else
			{
				if (m_go->pos.x < m_go->nearest->pos.y) // Up
					m_go->target.y += m_gridSize;
				else // Down
					m_go->target.y -= m_gridSize;
			}
		}
		else
		{
			dir.Normalize();
			m_go->pos += dir * static_cast<float>(dt) *  m_go->movespeed;
		}
	}


}

void StateNaughty::Exit()
{
}

StateHappy::StateHappy(const std::string & stateID, GameObject * go)
	:State(stateID),
	m_go(go)
{
}

StateHappy::~StateHappy()
{
}

void StateHappy::Enter()
{
	m_go->movespeed = 10.f;
}

void StateHappy::Update(double dt)
{
	// Check fish count states
	if (SceneData::GetInstance()->GetFishCount() >= 5)
		m_go->sm->SetNextState("StateNaughty");

	// Normal Movement
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
		if (random > 0.5f)
		{
			if (random > 0.75f) // Down
				m_go->target.y -= m_gridSize;
			else  // Right
				m_go->target.x += m_gridSize;
		}
		else if (random < 0.5f)
		{
			if (random < 0.25f) // Left
				m_go->target.x -= m_gridSize;
			else // Up
				m_go->target.y += m_gridSize;
		}
	}
	else
	{
		dir.Normalize();
		m_go->pos += dir * static_cast<float>(dt) * m_go->movespeed;
	}
}

void StateHappy::Exit()
{
}
