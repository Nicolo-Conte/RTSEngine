#pragma once
#include "Unit.h"
#include "Building.h"

/// <summary>
/// child class of Unit, and Base class of MeleeUnit and RangedUnit
/// is the only unit that can attack, but cannot Build or Collect.                        
/// </summary>
class CombatUnit : public Unit, public std::enable_shared_from_this<CombatUnit>
{
protected:
	GameObject* m_TargetEnemy;		//Pointer of the current Enemy building or unit assigned to the CombatUnit which have to attack

public:
	//Constructors & Destructor
	CombatUnit();
	CombatUnit(std::shared_ptr<PathManager> a_PathManager, std::shared_ptr<GameObjectManager> a_GameObjectManager, std::shared_ptr<Player> a_Player);
	~CombatUnit();

	//Setter & Getter
	GameObject* GetTargetEnemy();
	void SetTargetEnemy(GameObject* a_TargetEnemy);

	//other functions
	void StartAttack(GameObject* a_TargetEnemy);
	void AttackPreparation();
	virtual void AttackAction(float a_DeltaTime) = 0;
	virtual bool CheckDistance() = 0;
};

inline GameObject* CombatUnit::GetTargetEnemy()
{
	return m_TargetEnemy;
}

inline void CombatUnit::SetTargetEnemy(GameObject* a_TargetEnemy)
{
	m_TargetEnemy = a_TargetEnemy;
}