#include "StateDefault.h"

StateDefault::StateDefault(const std::string & stateID, GameObject * go)
	:State(stateID),
	m_go(go)
{
}

StateDefault::~StateDefault()
{
}

void StateDefault::Enter()
{
	
}

void StateDefault::Update(double dt)
{
}

void StateDefault::Exit()
{
}