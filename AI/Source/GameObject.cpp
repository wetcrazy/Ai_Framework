#include "GameObject.h"

// States in GameObject
//void GameObject::Update(double dt)
//{
//	switch (currState)
//	{
//	case GameObject::STATE_TOOFULL:
//		if (energy >= 20)
//		{
//			energy = 20;
//		}
//		energy -= 0.01f * (float)dt;
//		movespeed = 0.f;
//		if (energy < 15.f)
//			currState = GameObject::FISH_STATE::STATE_FULL;
//		break;
//	case GameObject::STATE_FULL:
//		energy -= 0.01f * (float)dt;
//		movespeed = 5.f;
//		if (energy < 10.f)
//			currState = GameObject::FISH_STATE::STATE_HUNGRY;
//		else if (energy >= 15.f)
//			currState = GameObject::FISH_STATE::STATE_TOOFULL;
//		break;
//	case GameObject::STATE_HUNGRY:
//		energy -= 0.01f * (float)dt;
//		movespeed = 10.f;
//		if (energy <= 0.f)
//			currState = GameObject::FISH_STATE::STATE_DEAD;
//		else if (energy >= 10.f)
//			currState = GameObject::FISH_STATE::STATE_FULL;
//		break;
//	case GameObject::STATE_DEAD:
//		energy -= 0.01f * (float)dt;
//		countDown -= 0.1f * (float)dt;
//		movespeed = 0.f;
//		energy = 0.f;
//		if (countDown < 0)
//		{
//			active = false;
//			--totalnumber;
//		}
//		break;
//	}
//
//}

GameObject::GameObject(GAMEOBJECT_TYPE typeValue)
	: type(typeValue),
	scale(1, 1, 1),
	active(false),
	mass(1.f),
	sm(NULL),
	nearest(NULL),
	timer(1.f),
	bed_location(0.f,0.f,0.f)
{
}

GameObject::~GameObject()
{
	if (sm != NULL)
	{
		delete sm;
	}
}