#ifndef STATE_PRISONERS_H
#define STATE_PRISONERS_H

#include "State.h"
#include "GameObject.h"

class StatePrisoners_Sleep : public State
{
	GameObject *m_go;
public:
	StatePrisoners_Sleep(const std::string &stateID, GameObject *go);
	virtual ~StatePrisoners_Sleep();

	virtual void Enter();
	virtual void Update(double dt);
	virtual void Exit();
};

class StatePrisoners_RoleCall : public State
{
	GameObject *m_go;
public:
	StatePrisoners_RoleCall(const std::string &stateID, GameObject *go);
	virtual ~StatePrisoners_RoleCall();

	virtual void Enter();
	virtual void Update(double dt);
	virtual void Exit();
};

class StatePrisoners_Escape : public State
{
	GameObject *m_go;
public:
	StatePrisoners_Escape(const std::string &stateID, GameObject *go);
	virtual ~StatePrisoners_Escape();

	virtual void Enter();
	virtual void Update(double dt);
	virtual void Exit();
};

class StatePrisoners_Exercise : public State
{
	GameObject *m_go;
public:
	StatePrisoners_Exercise(const std::string &stateID, GameObject *go);
	virtual ~StatePrisoners_Exercise();

	virtual void Enter();
	virtual void Update(double dt);
	virtual void Exit();
};

class StatePrisoners_Fight : public State
{
	GameObject *m_go;
public:
	StatePrisoners_Fight(const std::string &stateID, GameObject *go);
	virtual ~StatePrisoners_Fight();

	virtual void Enter();
	virtual void Update(double dt);
	virtual void Exit();
};


#endif