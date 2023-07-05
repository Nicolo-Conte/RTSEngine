#pragma once
#include "EntityManager/Faction.h"
#include "BuilderUnit.h"
#include "MeleeUnit.h"
#include "RangedUnit.h"
#include "House.h"
#include "Barrack.h"

class Player
{
public:
	//Constructors & Destructor
	Player() {}
	Player(std::shared_ptr<Faction> a_Faction);
	~Player() {}

	//Setters
	void SetCurrentBuilderUnits(std::vector<std::shared_ptr<BuilderUnit>> a_CurrentBuilderUnits);
	void SetCurrentCombatUnits(std::vector<std::shared_ptr<CombatUnit>> a_CurrentCombatUnits);
	void SetCurrentHouse(std::vector<std::shared_ptr<House>> a_CurrentHouse);
	void SetCurrentBarrack(std::vector<std::shared_ptr<Barrack>> a_CurrentBarrack);
	void SetBuildingUnderAttack(bool a_UnderAttack);
	

	//Setter
	std::vector<std::shared_ptr<BuilderUnit>> GetCurrentBuilderUnits();
	std::vector<std::shared_ptr<CombatUnit>> GetCurrentCombatUnits();
	std::vector<std::shared_ptr<House>> GetCurrentHouse();
	std::vector<std::shared_ptr<Barrack>> GetCurrentBarrack();
	std::shared_ptr<Faction> GetFaction();
	int GetCurrentPopulation();
	int GetMaxPopulation();


	//Other methods
	void AddBuilderUnit(std::shared_ptr<BuilderUnit> a_BuilderUnit);
	void AddCombatUnit(std::shared_ptr<CombatUnit> a_CombatUnit);
	void AddHouse(std::shared_ptr<House> a_House);
	void AddBarrack(std::shared_ptr<Barrack> a_Barrack);
	void AddMaxPopulation(int a_Increase);
	void AddPopulation(int a_Increase);
	void AddHouseUnderConstruction(bool a_HouseUnderConstruction);

	void DeleteBuilderUnit(std::shared_ptr<BuilderUnit> a_BuilderUnit);
	void DeleteCombatUnit(std::shared_ptr<CombatUnit> a_CombatUnit);
	void DeleteHouse(std::shared_ptr<House> a_House);
	void DeleteBarrack(std::shared_ptr<Barrack> a_Barrack);
	void DeleteHouseUnderConstruction();

protected:
	std::vector<std::shared_ptr<BuilderUnit>> m_CurrentBuilderUnits;
	std::vector<std::shared_ptr<CombatUnit>> m_CurrentCombatUnits;
	std::vector<std::shared_ptr<House>> m_CurrentHouse;
	std::vector<std::shared_ptr<Barrack>> m_CurrentBarrack;
	std::shared_ptr<Faction> m_Faction;

	int m_CurrentPopulation;			//current number of units of the player's faction
	int m_MaxPopulation;				//Max number of units of the player's faction  
	bool m_BuildingUnderAttack;			//Boolean having the function of notifying the player that a building is under attack
	bool m_UnitUnderAttack;				//Boolean having the function of notifying the player that a building is under attack
	std::vector<bool> m_HouseUnderConstruction;
	bool m_LoseMatch;
};

inline std::vector<std::shared_ptr<BuilderUnit>> Player::GetCurrentBuilderUnits()
{
	return m_CurrentBuilderUnits;
}

inline void Player::SetCurrentBuilderUnits(std::vector<std::shared_ptr<BuilderUnit>> a_CurrentBuilderUnits)
{
	m_CurrentBuilderUnits = a_CurrentBuilderUnits;
}

inline std::vector<std::shared_ptr<CombatUnit>> Player::GetCurrentCombatUnits()
{
	return m_CurrentCombatUnits;
}

inline void Player::SetCurrentCombatUnits(std::vector<std::shared_ptr<CombatUnit>> a_CurrentCombatUnits)
{
	m_CurrentCombatUnits = a_CurrentCombatUnits;
}

inline std::vector<std::shared_ptr<House>> Player::GetCurrentHouse()
{
	return m_CurrentHouse;
}

inline void Player::SetCurrentHouse(std::vector<std::shared_ptr<House>> a_CurrentHouse)
{
	m_CurrentHouse = a_CurrentHouse;
}

inline std::vector<std::shared_ptr<Barrack>> Player::GetCurrentBarrack()
{
	return m_CurrentBarrack;
}

inline void Player::SetCurrentBarrack(std::vector<std::shared_ptr<Barrack>> a_CurrentBarrack)
{
	m_CurrentBarrack = a_CurrentBarrack;
}

inline void Player::SetBuildingUnderAttack(bool a_UnderAttack)
{
	m_BuildingUnderAttack = a_UnderAttack;
}

inline void Player::AddHouseUnderConstruction(bool a_HouseUnderConstruction)
{
	m_HouseUnderConstruction.push_back(a_HouseUnderConstruction);
}

inline std::shared_ptr<Faction> Player::GetFaction()
{
	return m_Faction;
}

inline int Player::GetCurrentPopulation()
{
	return m_CurrentPopulation;
}

inline int Player::GetMaxPopulation()
{
	return m_MaxPopulation;
}
