#ifndef STATE_COP_H
#define STATE_COP_H

#include "State.h"
#include "GameObject.h"


class StateCop_Patrol : public State
{
	GameObject *m_go;
public:
	StateCop_Patrol(const std::string &stateID, GameObject *go);
	virtual ~StateCop_Patrol();

	virtual void Enter();
	virtual void Update(double dt);
	virtual void Exit();
};

class StateCop_Rest : public State
{
	GameObject *m_go;
public:
	StateCop_Rest(const std::string &stateID, GameObject *go);
	virtual ~StateCop_Rest();

	virtual void Enter();
	virtual void Update(double dt);
	virtual void Exit();
};

class StateCop_Fight : public State
{
	GameObject *m_go;
public:
	StateCop_Fight(const std::string &stateID, GameObject *go);
	virtual ~StateCop_Fight();

	virtual void Enter();
	virtual void Update(double dt);
	virtual void Exit();
};

class StateCop_Capture : public State
{
	GameObject *m_go;
public:
	StateCop_Capture(const std::string &stateID, GameObject *go);
	virtual ~StateCop_Capture();

	virtual void Enter();
	virtual void Update(double dt);
	virtual void Exit();
};

#endif