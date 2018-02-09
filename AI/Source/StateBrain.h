#ifndef STATE_BRAIN_H
#define STATE_BRAIN_H

#include "State.h"
#include "GameObject.h"

// It is a Default State for no AI
class StateBrain : public State
{
	GameObject *m_go;
public:
	StateBrain(const std::string &stateID, GameObject *go);
	virtual ~StateBrain();

	virtual void Enter();
	virtual void Update(double dt);
	virtual void Exit();
};


#endif