#include "CombatUnit.h"

CombatUnit::CombatUnit() :
	m_TargetEnemy(nullptr)
{}

CombatUnit::CombatUnit(std::shared_ptr<PathManager> a_PathManager, std::shared_ptr<GameObjectManager> a_GameObjectManager, std::shared_ptr<Player> a_Player) :
Unit(a_PathManager, a_GameObjectManager, a_Player),
m_TargetEnemy(nullptr)
{}

CombatUnit::~CombatUnit()
{}

/// <summary>
/// method that prepares a CombatUnit to Attack, specifying Enemy target
/// </summary>
/// <param name="a_TargetEnemy">current Enemy building or unit assigned to the CombatUnit which have to attack</param>
void CombatUnit::StartAttack(GameObject* a_TargetEnemy)
{
	if (a_TargetEnemy->GetPlayer() != m_Player)
	{
		if (m_CurrentState != InAction)
		{
			m_CurrentState = InAction;
		}
		m_TargetEnemy = a_TargetEnemy;
		m_TargetPosition = a_TargetEnemy->GetTransform().Position;
		BuildPath(m_TargetPosition);
		if (dynamic_cast<CombatUnit*>(a_TargetEnemy) != nullptr)		//check if the target enemy is a CombatUnit object or not
		{
			if (a_TargetEnemy->GetUnderAttack() == false)				//if the targetEnemy is a CombatUnit and is not currently attacking, set the targetEnemy passing the CombatUnit reference
			{
				dynamic_cast<CombatUnit*>(a_TargetEnemy)->SetTargetEnemy(this);
			}
		}
	}
	return;
	
}


/// <summary>
/// 
/// </summary>
void CombatUnit::AttackPreparation()
{
	m_TargetPosition = m_TargetEnemy->GetTransform().Position;
	BuildPath(m_TargetPosition);
	m_CurrentState = InAction;
	return;
}