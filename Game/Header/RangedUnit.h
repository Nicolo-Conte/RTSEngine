#pragma once
#include "CombatUnit.h"

/// <summary>
/// child class of CombatUnit, possible action:
/// 1. Move 
/// 2. Range Attack 
/// </summary>
class RangedUnit : public CombatUnit
{
private:
public:
	//Constructors & Destructor
	RangedUnit(std::shared_ptr<PathManager> a_PathManager, glm::vec2 a_Position, std::shared_ptr<GameObjectManager> a_GameObjectManager, std::shared_ptr<Player> a_Player);
	~RangedUnit();

	//Setter & Getter

	//other functions
	bool CheckDistance();
	void AttackAction(float a_DeltaTime);
	void Update(float a_DeltaTime);
	bool Move(float a_DeltaTime);
};

