#include "Player.h"


Player::Player(std::shared_ptr<Faction> a_Faction) :
	m_CurrentBuilderUnits(0),
	m_CurrentCombatUnits(0),
	m_CurrentHouse(0),
	m_CurrentBarrack(0),
	m_Faction(a_Faction),
	m_CurrentPopulation(0),
	m_MaxPopulation(MAXPOPULATIONSTART),
	m_BuildingUnderAttack(false),
	m_UnitUnderAttack(false),
	m_LoseMatch(false)
{}


void Player::AddBuilderUnit(std::shared_ptr<BuilderUnit> a_BuilderUnit)
{
	m_CurrentBuilderUnits.push_back(a_BuilderUnit);
	m_Faction->AddUnit(a_BuilderUnit);
	m_CurrentPopulation++;
}

void Player::AddCombatUnit(std::shared_ptr<CombatUnit> a_CombatUnit)
{
	m_CurrentCombatUnits.push_back(a_CombatUnit);
	m_Faction->AddUnit(a_CombatUnit);
	m_CurrentPopulation++;
}

void Player::AddHouse(std::shared_ptr<House> a_House)
{
	m_CurrentHouse.push_back(a_House);
	m_Faction->AddBuilding(a_House);
	m_MaxPopulation+=INCREASEMAXPOPULATION;
}

void Player::AddBarrack(std::shared_ptr<Barrack> a_Barrack)
{
	m_CurrentBarrack.push_back(a_Barrack);
	m_Faction->AddBuilding(a_Barrack);
}

void Player::AddMaxPopulation(int a_Increase)
{
	m_MaxPopulation += a_Increase;
}

void Player::AddPopulation(int a_Increase)
{
	m_CurrentPopulation += a_Increase;
}


void Player::DeleteBuilderUnit(std::shared_ptr<BuilderUnit> a_BuilderUnit)
{
	for (size_t i = 0; i < m_CurrentBuilderUnits.size(); i++)
	{
		if (a_BuilderUnit == m_CurrentBuilderUnits[i])
		{
			m_CurrentBuilderUnits.erase(m_CurrentBuilderUnits.begin() + i);
			break;
		}
	}
	m_Faction->RemoveUnit(a_BuilderUnit);
	m_CurrentPopulation--;
}

void Player::DeleteCombatUnit(std::shared_ptr<CombatUnit> a_CombatUnit)
{
	for (size_t i = 0; i < m_CurrentCombatUnits.size(); i++)
	{
		if (a_CombatUnit == m_CurrentCombatUnits[i])
		{
			m_CurrentCombatUnits.erase(m_CurrentCombatUnits.begin() + i);
			break;
		}
	}
	m_Faction->RemoveUnit(a_CombatUnit);
	m_CurrentPopulation--;
}


void Player::DeleteHouse(std::shared_ptr<House> a_House)
{
	for (size_t i = 0; i < m_CurrentHouse.size(); i++)
	{
		if (a_House == m_CurrentHouse[i])
		{
			m_CurrentHouse.erase(m_CurrentHouse.begin() + i);
			break;
		}
	}
	m_Faction->RemoveBuilding(a_House);
	m_MaxPopulation -= INCREASEMAXPOPULATION;
}

void Player::DeleteBarrack(std::shared_ptr<Barrack> a_Barrack)
{
	for (size_t i = 0; i < m_CurrentBarrack.size(); i++)
	{
		if (a_Barrack == m_CurrentBarrack[i])
		{
			m_CurrentBarrack.erase(m_CurrentBarrack.begin() + i);
			break;
		}
	}
	//m_Faction->RemoveBuilding(a_Barrack);
}

void Player::DeleteHouseUnderConstruction()
{
	//m_HouseUnderConstruction.pop_back();
}
