#pragma once
#include "Building.h"
#include <vector>
/// <summary>
/// The building 'Barrack' it's the only buildings which can produce Combat Units:
/// 1.	Melee Unit
/// 2.	Ranged Unit
/// can have a maximum production queue of 20 units
/// </summary>
class Barrack : public Building, public std::enable_shared_from_this<Barrack>
{
private:

	std::vector<int> m_UnitQueue;		//Vector containing the different types of units to be produced. 0 --> Nothing , 1 --> MeleeUnit , 2 --> RangedUnit

public:
	//Constructors & Destructor
	Barrack(glm::vec2 a_Position, std::shared_ptr<GameObjectManager> m_GameObjectManager, std::shared_ptr<Player> a_Player);
	~Barrack();

	//Setter & Getter
	std::vector<int> GetUnitQueue();
	void SetUnitQueue(std::vector<int> a_UnitQueue);

	//other functions
	void ProduceUnit() {}
	void ProduceUnit(int a_UnitType);
	void ProduceAction(float a_DeltaTime);
	void Update(float a_DeltaTime);
};

inline std::vector<int> Barrack::GetUnitQueue()
{
	return m_UnitQueue;
}

inline void Barrack::SetUnitQueue(std::vector<int> a_UnitQueue)
{
	m_UnitQueue = a_UnitQueue;
}
