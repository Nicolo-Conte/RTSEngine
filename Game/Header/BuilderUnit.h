#pragma once
#include "Unit.h"
#include "Resource.h"
#include "Building.h"
/// <summary>
/// child class of Unit, 
/// is the only unit that can’t attack.                        
/// </summary>
class BuilderUnit : public Unit, public std::enable_shared_from_this<BuilderUnit>
{
private:
	
	int m_ResourcesCollected;				//Current amount of resources collected by the unit
	Resource* m_Resource;	//resource point which the unit is collecting
	glm::vec2 m_HousePosition;				//Position of the House building which the builder have to bring the resources collected
	bool m_Building;						//Boolean values which indicate if the Unit is currently building
	bool m_Collecting;						//Boolean values which indicate if the Unit is currently collecting resources
	enum CollectingState					//State which indicates if the builder is collecting resources or bring them to the deposit(House)
	{
		Collect = 0,
		Deposit,
	};
	float m_BuildingTime;					//construction time counter
	int m_BuildingType;						//indicates which building to build. 0 -> Nothing 1 -> House, 2 -> Barrack.
	CollectingState m_CollectingState;		//Current state of the builder which is executing the collecting action
	std::shared_ptr<Building> m_BuildingTarget;

public:

	//Constructors & Destructor
	BuilderUnit(std::shared_ptr<PathManager> a_PathManager, std::shared_ptr<GameObjectManager> a_GameObjectManager, glm::vec2 a_Position, std::shared_ptr<Player> a_Player);
	BuilderUnit(int a_ResourcesCollected, Resource* a_ResourcePosition, bool a_Building, bool a_Collecting);
	~BuilderUnit();

	//Setter & Getter
	int GetResourcesCollected();
	Resource* GetResourcePosition();
	glm::vec2 GetHousePosition();
	bool GetBuilding();
	bool GetCollecting();
	float GetBuildingTime();
	int GetBuildingType();
	UnitState GetState();
	void SetResourcesCollected(int a_ResourcesCollected);
	void SetResourcePosition(Resource* a_ResourcePosition);
	void SetHousePosition(glm::vec2 a_HousePosition);
	void SetBuilding(bool a_Building);
	void SetCollecting(bool a_Collecting);
	void SetBuildingTime(float a_BuildingTime);
	void SetBuildingType(int a_BuildingType);

	//other functions
	void Update(float a_DeltaTime);
	void StartCollect(Resource* a_ResourcePosition);
	void StartBuild(std::shared_ptr<Building> a_TargetPosition, int a_BuildingType);
	void StartMove(glm::vec2 a_TargetPosition);
	void BuildAction(float a_DeltaTime);
	void CollectAction(float a_DeltaTime);
	bool Move(float a_DeltaTime);
};



inline int BuilderUnit::GetResourcesCollected()
{
	return m_ResourcesCollected;
}

inline Resource* BuilderUnit::GetResourcePosition()
{
	return m_Resource;
}

inline glm::vec2 BuilderUnit::GetHousePosition()
{
	return m_HousePosition;
}

inline bool BuilderUnit::GetBuilding()
{
	return m_Building;
}

inline bool BuilderUnit::GetCollecting()
{
	return m_Collecting;
}

inline float BuilderUnit::GetBuildingTime()
{
	return m_BuildingTime;
}

inline int BuilderUnit::GetBuildingType()
{
	return m_BuildingType;
}

inline Unit::UnitState BuilderUnit::GetState()
{
	return m_CurrentState;
}

inline void BuilderUnit::SetResourcesCollected(int a_ResourcesCollected)
{
	m_ResourcesCollected = a_ResourcesCollected;
}

inline void BuilderUnit::SetResourcePosition(Resource* a_ResourcePosition)
{
	m_Resource = a_ResourcePosition;
}

inline void BuilderUnit::SetHousePosition(glm::vec2 a_HousePosition)
{
	m_HousePosition = a_HousePosition;
}

inline void BuilderUnit::SetBuilding(bool a_Building)
{
	m_Building = a_Building;
}

inline void BuilderUnit::SetCollecting(bool a_Collecting)
{
	m_Collecting = a_Collecting;

}

inline void BuilderUnit::SetBuildingTime(float a_BuildingTime)
{
	m_BuildingTime = a_BuildingTime;
}

inline void BuilderUnit::SetBuildingType(int a_BuildingType)
{
	m_BuildingType = a_BuildingType;
}