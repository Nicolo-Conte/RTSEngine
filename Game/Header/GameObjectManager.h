#pragma once
#include"Barrack.h"
#include "House.h"
#include "MeleeUnit.h"
#include "RangedUnit.h"
#include "BuilderUnit.h"
#include "EntityManager.h"
#include "Resource.h"
#include "EnemyPlayer.h"

class PlayerControls;
class GameObjectManager : public std::enable_shared_from_this<GameObjectManager>
{
public:
	GameObjectManager(std::shared_ptr<EntityManager> a_EntityManager, std::shared_ptr<PathManager> a_PathManager);
	~GameObjectManager();

	enum SelectState
	{
		NoSelection = 0,
		SelectedtCombatUnits,
		SelectedBuilderUnits,
		SelectedHouse,
		SelectedBarrack
	};

	//Setters
	void SetCombatUnit(CombatUnit* a_CombatUnitsSelected);
	void SetBuilderUnit(BuilderUnit* a_BuilderUnitsSelected);
	void SetHouse(House* a_House);
	void SetBarrack(Barrack* a_Barrack);
	void SetPositionTarget(glm::vec2 a_Target);
	void SetNoSelection();
	//Getters
	CombatUnit* GetCombatUnit();
	BuilderUnit* GetBuilderUnit();
	House* GetHouse();
	Barrack* GetBarrack();
	glm::vec2 GetPositionTarget();

	void AddPlayer(std::shared_ptr<Player> a_Player);
	void AddEnemyPlayer(std::shared_ptr<EnemyPlayer> a_Player);
	void AddResource(std::shared_ptr<Resource> a_ResourcePoint);
	std::shared_ptr<EnemyPlayer> GetEnemy() { return m_EnemyPlayersInGame[0]; };
	//void Update(float a_DeltaTime);
	void ExecuteOrders(GameObject* a_Target);
	void ExecuteOrders(glm::vec2 a_PositionTarget);
	void ExecuteOrders(int a_UnitType);

	void ExecuteEnemyOrders(std::shared_ptr<GameObject> a_Target, std::vector<std::shared_ptr<CombatUnit>> a_CombatUnitsSelected);
	void ExecuteEnemyOrders(std::shared_ptr<GameObject> a_Target, std::vector<std::shared_ptr<BuilderUnit>> a_SelectedBuilders);
	void ExecuteEnemyOrders(std::shared_ptr<House> a_House);
	void ExecuteEnemyOrders(int a_UnitType, std::shared_ptr<Barrack> a_Barrack);

	void SetPlayerControls(std::shared_ptr<PlayerControls> a_PlayerControls);
	void PlaceBuilding(glm::vec2 a_Building, int a_BuildingType, std::shared_ptr<Player> a_Player);
	void PlaceEnemyBuilding(glm::vec2 a_Building, int a_BuildingType, std::shared_ptr<Player> a_Player, std::vector<std::shared_ptr<BuilderUnit>> a_SelectedBuilders);
	glm::vec2 SearchDeposit(glm::vec2 a_ResourcePosition, std::shared_ptr<Player> a_Player);
	std::shared_ptr<Resource> SearchResourcePosition(std::shared_ptr<Player> a_Player);
	std::shared_ptr<Player> GetPlayer();
	void Update(float a_DeltaTime);

	void WarnPlayerControls();

	void CreateUnit(glm::vec2 a_UnitPosition, int a_TypeUnit, std::shared_ptr<Player> a_Player);
	void DeleteObject(std::shared_ptr<GameObject> a_GameObject);
	std::vector<std::shared_ptr<GameObject>> GetTargetsToAttack(std::shared_ptr<Player> a_Player);

private:

	CombatUnit* m_CombatUnitsSelected;		//BuilderUnit that the user has selected
	BuilderUnit* m_BuilderUnitsSelected;	//CombatUnit that the user has selected
	House* m_HouseSelected;								//House that the user has selected
	Barrack* m_BarrackSelected;							//Barrack that the user has selected


	std::vector<std::shared_ptr<Resource>> m_ResourcePoint;
	glm::vec2 m_PositionTarget;											//Target position to be reached by the selected units

	std::vector<std::shared_ptr<EnemyPlayer>> m_EnemyPlayersInGame;
	std::shared_ptr<PlayerControls> m_PlayerControls;
	std::shared_ptr<EntityManager> m_EntityManager;
	std::shared_ptr<PathManager> m_PathManager;
	std::vector<std::shared_ptr<Player>> m_PlayersInGame;
	SelectState m_CurrentState;
};

inline void GameObjectManager::SetCombatUnit(CombatUnit* a_CombatUnitsSelected)
{
	m_CombatUnitsSelected = a_CombatUnitsSelected;
	m_CurrentState = SelectedtCombatUnits;
}

inline CombatUnit* GameObjectManager::GetCombatUnit()
{
	return m_CombatUnitsSelected;
}

inline void GameObjectManager::SetBuilderUnit(BuilderUnit* a_BuilderUnitsSelected)
{
	m_BuilderUnitsSelected = a_BuilderUnitsSelected;
	m_CurrentState = SelectedBuilderUnits;
}

inline BuilderUnit* GameObjectManager::GetBuilderUnit()
{
	return m_BuilderUnitsSelected;
}

inline void GameObjectManager::SetHouse(House* a_House)
{
	m_HouseSelected = a_House;
	m_CurrentState = SelectedHouse;
}

inline House* GameObjectManager::GetHouse()
{
	return m_HouseSelected;
}

inline void GameObjectManager::SetBarrack(Barrack* a_Barrack)
{
	m_BarrackSelected = a_Barrack;
	m_CurrentState = SelectedBarrack;
}

inline Barrack* GameObjectManager::GetBarrack()
{
	return m_BarrackSelected;
}

inline void GameObjectManager::SetPositionTarget(glm::vec2 a_Target)
{
	m_PositionTarget = a_Target;
}

inline glm::vec2 GameObjectManager::GetPositionTarget()
{
	return m_PositionTarget;
}

inline void GameObjectManager::SetNoSelection()
{
	m_CurrentState = NoSelection;
	 m_CombatUnitsSelected  = nullptr;		
	 m_BuilderUnitsSelected = nullptr;	
	 m_HouseSelected        = nullptr;			
	 m_BarrackSelected      = nullptr;
}