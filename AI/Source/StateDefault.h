#ifndef STATE_DEFAULT_H
#define STATE_DEFAULT_H

#include "State.h"
#include "GameObject.h"

// It is a Default State for no AI
class StateDefault : public State
{
	GameObject *m_go;
public:
	StateDefault(const std::string &stateID, GameObject *go);
	virtual ~StateDefault();

	virtual void Enter();
	virtual void Update(double dt);
	virtual void Exit();
};


#endif