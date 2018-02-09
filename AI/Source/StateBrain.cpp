#include "StateBrain.h"

StateBrain::StateBrain(const std::string & stateID, GameObject * go)
	:State(stateID),
	m_go(go)
{
}

StateBrain::~StateBrain()
{
}

void StateBrain::Enter()
{
	
}

void StateBrain::Update(double dt)
{
}

void StateBrain::Exit()
{
}