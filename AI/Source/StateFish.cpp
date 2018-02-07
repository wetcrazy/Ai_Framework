#include "StateFish.h"

#include "PostOffice.h"
#include "ConcreteMessages.h" 

// STATE TOO FULL
StateTooFull::StateTooFull(const std::string & stateID, GameObject * go)
	: State(stateID),
	m_go(go)
{
}

StateTooFull::~StateTooFull()
{
}

void StateTooFull::Enter()
{
	m_go->target = m_go->pos;
	m_go->movespeed = 0.f;
}

void StateTooFull::Update(double dt)
{
	// Variable updater
	if (m_go->energy >= 20)
		m_go->energy = 20;
	m_go->energy -= 0.5f *(float)dt;

	// Check energy states
	if (m_go->energy < 15.f)
		m_go->sm->SetNextState("StateFull");

}

void StateTooFull::Exit()
{
}

// STATE FULL
StateFull::StateFull(const std::string & stateID, GameObject * go)
	:State(stateID),
	m_go(go)
{
}

StateFull::~StateFull()
{
}

void StateFull::Enter()
{
	m_go->target = m_go->pos;
	m_go->movespeed = 5.f;
}

void StateFull::Update(double dt)
{
	// Variable updater
	m_go->energy -= 0.5f *(float)dt;
	m_go->timer += (float)dt;

	// Check energy states
	if (m_go->energy < 10.f)
		m_go->sm->SetNextState("StateHungry");
	else if (m_go->energy >= 15.f)
		m_go->sm->SetNextState("StateTooFull");

	if (m_go->timer >= 1.f)
	{
		PostOffice::GetInstance()->Send("SceneMovement", new MessageWRU(m_go, MessageWRU::NEAREST_SHARK, m_go->scale.x*8.f));
		m_go->timer = 0.f;
	}

	// Check nearest object
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
			if (random >= 0.5f)
			{
				if (m_go->pos.x > m_go->nearest->pos.x) // Right
					m_go->target.x += m_gridSize;
				else  // Left
					m_go->target.x -= m_gridSize;
			}
			else
			{
				if (m_go->pos.x > m_go->nearest->pos.y) // Up
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
	// Normal movement
	else
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
}

void StateFull::Exit()
{
}

// STATE HUNGRY
StateHungry::StateHungry(const std::string & stateID, GameObject * go)
	:State(stateID),
	m_go(go)
{
}

StateHungry::~StateHungry()
{
}

void StateHungry::Enter()
{
	m_go->target = m_go->pos;
	m_go->movespeed = 10.f;
}

void StateHungry::Update(double dt)
{
	// Variable updater
	m_go->energy -= 0.5f * (float)dt;

	// Check energy states
	if (m_go->energy <= 0.f)
		m_go->sm->SetNextState("StateDead");
	else if (m_go->energy >= 10.f)
		m_go->sm->SetNextState("StateFull");

	// Find Nearest fish food and hunt it
	if (m_go->nearest == NULL)
		PostOffice::GetInstance()->Send("SceneMovement", new MessageWRU(m_go, MessageWRU::NEAREST_FISHFOOD, m_go->scale.x * 10.f));
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
	// Normal movement
	else
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
}

void StateHungry::Exit()
{
}

// STATE DEAD
StateDead::StateDead(const std::string & stateID, GameObject * go)
	:State(stateID),
	m_go(go)
{
}

StateDead::~StateDead()
{
}

void StateDead::Enter()
{
	m_go->movespeed = 0.f;
	m_go->energy = 0.f;
	SceneData::GetInstance()->SetFishCount(SceneData::GetInstance()->GetFishCount() - 1);
}

void StateDead::Update(double dt)
{
	m_go->countDown -= 0.5f * (float)dt;	
	if (m_go->countDown < 0)
	{
		m_go->active = false;		
	}
}

void StateDead::Exit()
{
}
