#include"SceneData.h"

SceneData::SceneData()
{
}

SceneData::~SceneData()
{
}

int SceneData::GetObjectCount()
{
	return m_objectCount;
}

void SceneData::SetObjectCount(int objectCount)
{
	m_objectCount = objectCount;
}

int SceneData::GetFishCount()
{
	return m_fishCount;
}

void SceneData::SetFishCount(int fishCount)
{
	m_fishCount = fishCount;
}

int SceneData::GetnoGrid()
{
	return m_noGrid;
}

void SceneData::SetnoGrid(int noGrid)
{
	m_noGrid = noGrid;
}

float SceneData::GetGridSize()
{
	return m_gridSize;
}

void SceneData::SetGridSize(float gridSize)
{
	m_gridSize = gridSize;
}

float SceneData::GetGridOffset()
{
	return m_gridOffset;
}

void SceneData::SetGridOffset(float gridoffset)
{
	m_gridOffset = gridoffset;
}

int SceneData::GetSharkKillCount()
{
	return m_sharkkillcount;
}

void SceneData::SetSharkKillCount(int sharkkillcount)
{
	m_sharkkillcount = sharkkillcount;
}

float SceneData::GetClockTime()
{
	return m_clocktime;
}

void SceneData::SetClockTime(float clocktime)
{
	m_clocktime = clocktime;
}

Vector3 SceneData::GetRoleCallPos()
{
	return m_rolecallPos;
}

void SceneData::SetRoleCallPos(Vector3 rolecallPos)
{
	m_rolecallPos = rolecallPos;
}

Vector3 SceneData::GetCop_Patrol_Nodes(int x)
{
	if (m_cop_patrol_nodes[x] == Vector3(0,0,0))
	{
		return 0;
	}
	else
		return m_cop_patrol_nodes[x];
}

void SceneData::SetCop_Patrol_Nodes(Vector3 cop_patrol_nodes[])
{
	for (int n = 0; n < sizeof(cop_patrol_nodes) / sizeof(*cop_patrol_nodes); ++n)
	{
		m_cop_patrol_nodes[n] = cop_patrol_nodes[n];
	}
}

bool SceneData::GetisFight_Start()
{
	return m_isFight_start;
}

void SceneData::SetisFight_Start(bool isFight_Start)
{
	m_isFight_start = isFight_Start;
}
