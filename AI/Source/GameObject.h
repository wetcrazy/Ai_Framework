#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "Vector3.h"
#include "StateMachine.h"
#include <vector>
#include "Maze.h"

struct GameObject
{
	enum GAMEOBJECT_TYPE
	{
		GO_NONE = 0,
		GO_BALL,
		GO_CROSS,
		GO_CIRCLE,
		GO_FOOD,
		GO_SHARK,
		GO_FISH,

		// Assignment 1
		GO_PRISONERS,
		GO_COPS,

		GO_QUEEN,

		GO_NPC,

		GO_RED,
		GO_BLUE,
		GO_NETURAL,

		GO_TOTAL, //must be last
	};
	/*enum FISH_STATE
	{
		STATE_NONE=0,
		STATE_TOOFULL,
		STATE_FULL,
		STATE_HUNGRY,
		STATE_DEAD,
		STATE_TOTAL,
	};*/
	/*enum STATE
	{
		NONE = 0,
		STATE_FISH,
		STATE_DEFAULT,
		TOTAL,
	}; */

	// States in GameObject
	//void Update(double dt);

	// States in state classes
	StateMachine *sm;

	GAMEOBJECT_TYPE type;
	Vector3 pos;
	Vector3 vel;
	Vector3 scale;
	bool active;
	float mass;
	Vector3 target;
	int id;
	int steps;
	// int totalnumber;

	// Fishy and Shark
	float energy;
	float movespeed;
	float countDown; // Fish despawner
	GameObject *nearest; // Nearest object near the curr object
	float timer;

	// Assingment 1 Prisoners
	Vector3 bed_location;

	GameObject(GAMEOBJECT_TYPE typeValue = GO_BALL);
	~GameObject();

	std::vector<Maze::TILE_CONTENT> grid;
	std::vector<bool> visited;
	std::vector<MazePt> stack;
	std::vector<MazePt> path;
	MazePt curr;

	// For graph movement
	unsigned currNodeID;
	std::vector<unsigned> gStack;
	std::vector<Vector3> gPath;
	//Node currNode;

	// Lab 11
	unsigned Lab11_factionType;
	unsigned Lab11_unitType;
	float Lab11_hp;
	float Lab11_range;
	float Lab11_damage;
	float Lab11_actionTime;
};

#endif