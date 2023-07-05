#pragma once
#include "Building.h"

/// <summary>
/// The building “House” have three different functions:
/// 1. Increase the max population of 5;
/// 2. Is used by the BuilderUnit as a warehouse to bring the resources collected.
/// 3. House is the only building that can produce BuilderUnit
/// </summary>
class House : public Building, public std::enable_shared_from_this<House>
{
private:
	int m_UnitQueue;					//current number of units in production within the house
public:
	//Constructors & Destructor
	House(glm::vec2 a_Position, std::shared_ptr<GameObjectManager> a_GameObjectManager, std::shared_ptr<Player> a_Player);
	~House();

	//Setter & Getter
	int GetUnitQueue();
	void SetUnitQueue(int a_UnitQueue);
	void Build();
	//other functions
	void ProduceUnit();
	void ProduceAction(float a_DeltaTime);
	void Update(float a_DeltaTime);
};

inline int House::GetUnitQueue()
{
	return m_UnitQueue;
}

inline void House::SetUnitQueue(int a_UnitQueue)
{
	m_UnitQueue = a_UnitQueue;
}

