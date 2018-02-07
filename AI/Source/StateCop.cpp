#include "StateCop.h"

// Cop Patrol around compound
StateCop_Patrol::StateCop_Patrol(const std::string & stateID, GameObject * go)
	: State(stateID),
	m_go(go)
{
}
StateCop_Patrol::~StateCop_Patrol()
{
}
void StateCop_Patrol::Enter()
{
	m_go->target = m_go->pos;
	m_go->movespeed = 12.f;
}
void StateCop_Patrol::Update(double dt)
{
	if (m_go->energy <= 0 && !SceneData::GetInstance()->GetisFight_Start())
		m_go->sm->SetNextState("StateCop_Rest");
}
void StateCop_Patrol::Exit()
{
}