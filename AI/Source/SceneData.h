#ifndef SCENEDATA_H
#define SCENEDATA_H

#include "SingletonTemplate.h"
#include "Vector3.h"

class SceneData :public Singleton<SceneData>
{
	friend Singleton<SceneData>;
private:
	SceneData();
	~SceneData();
	int m_objectCount;
	int m_fishCount;
	int m_noGrid;
	float m_gridSize;
	float m_gridOffset;

	int m_sharkkillcount;

	// Assignment 1
	float m_clocktime;
	Vector3 m_rolecallPos;
	Vector3 m_cop_patrol_nodes[10];
	bool m_isFight_start;



public:
	int GetObjectCount();
	void SetObjectCount(int objectCount);

	int GetFishCount();
	void SetFishCount(int fishCount);

	int GetnoGrid();
	void SetnoGrid(int noGrid);

	float GetGridSize();
	void SetGridSize(float gridSize);

	float GetGridOffset();
	void SetGridOffset(float gridoffset);

	int GetSharkKillCount();
	void SetSharkKillCount(int sharkkillcount);

	float GetClockTime();
	void SetClockTime(float clocktime);

	Vector3 GetRoleCallPos();
	void SetRoleCallPos(Vector3 rolecallPos);

	// Get the cops patrols nodes starting from 0
	Vector3 GetCop_Patrol_Nodes(int x);
	void SetCop_Patrol_Nodes(Vector3 cop_patrol_nodes[]);

	bool GetisFight_Start();
	void SetisFight_Start(bool isFight_Start);
};
#endif // !SCENEDATA_H
