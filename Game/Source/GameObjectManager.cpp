#include "GameObjectManager.h"
#include "PlayerControls.h"
#include <memory>

GameObjectManager::GameObjectManager(std::shared_ptr<EntityManager> a_EntityManager, std::shared_ptr<PathManager> a_PathManager) :
	m_CombatUnitsSelected(0),
	m_BuilderUnitsSelected(0),
	m_HouseSelected(nullptr),
	m_BarrackSelected(nullptr),
	m_PlayerControls(nullptr),
	m_PositionTarget(glm::vec2(0.0f, 0.0f)),
	m_EntityManager(a_EntityManager),
	m_PathManager(a_PathManager),
	m_PlayersInGame(0),
	m_EnemyPlayersInGame(0),
	m_CurrentState(SelectState::NoSelection),
	m_ResourcePoint(0)
{}

GameObjectManager::~GameObjectManager()
{}

void GameObjectManager::AddPlayer(std::shared_ptr<Player> a_Player)
{
	m_PlayersInGame.push_back(a_Player);
}

void GameObjectManager::AddEnemyPlayer(std::shared_ptr<EnemyPlayer> a_Player)
{
	m_EnemyPlayersInGame.push_back(a_Player);
}

void GameObjectManager::AddResource(std::shared_ptr<Resource> a_ResourcePoint)
{
	m_ResourcePoint.push_back(a_ResourcePoint);
}

void GameObjectManager::Update(float a_DeltaTime)
{
	for(int i = 0; i< m_EnemyPlayersInGame.size();i++)
		m_EnemyPlayersInGame[i]->Update(a_DeltaTime);
}

void GameObjectManager::WarnPlayerControls()
{
	m_PlayerControls->SetSelectedObject(nullptr);
}

void GameObjectManager::ExecuteOrders(glm::vec2 a_PositionTarget)
{
	switch (m_CurrentState)
	{
	case 0:		//no object being selected
	{

		break;
	}
	case 1:		//one or more CombatUnits have been selected
	{
		
			m_CombatUnitsSelected->StartMove(a_PositionTarget);
		
		break;
	}
	case 2:		//one or more BuilderUnits have been selected
	{

			m_BuilderUnitsSelected->StartMove(a_PositionTarget);
		
		break;
	}
	default:
	{
		printf("GameObjectManager isn't in one of his states, or trying to give an Move order to a building");

		break;
	}
	}
}

//Note:
//Function contains dynamic_cast, because of this, .get() is used.
//Try to find another way?
void GameObjectManager::ExecuteOrders(GameObject* a_Target)
{
	switch (m_CurrentState)
	{
	case 0:		//no object being selected
	{

		break;
	}
	case 1:		//one or more CombatUnits have been selected
	{
		if ((dynamic_cast<Unit*>(a_Target) != nullptr)|| (dynamic_cast<Building*>(a_Target) != nullptr))
		{
			
				m_CombatUnitsSelected->StartAttack(a_Target);
			
		}
		break;
	}
	case 2:		//one or more BuilderUnits have been selected
	{

		if (dynamic_cast<Resource*>(a_Target) != nullptr)
		{
			
				m_BuilderUnitsSelected->StartCollect((Resource*)(a_Target));
			
		}
		break;
	}
	default:
	{
		printf("GameObjectManager isn't in one of his states");

		break;
	}
	}
}


void GameObjectManager::ExecuteOrders(int a_UnitType)
{
	switch (m_CurrentState)
	{
	case 0:		//no object being selected
	{

		break;
	}
	case 3:		//House have been selected
	{
		m_HouseSelected->ProduceUnit();

		break;
	}
	case 4:		//Barrack have been selected
	{

		m_BarrackSelected->ProduceUnit(a_UnitType);

		break;
	}
	default:
	{
		printf("GameObjectManager isn't in one of his states");

		break;
	}
	}
}

//Note:
//Function contains dynamic_cast, because of this, .get() is used.
//Try to find another way?
void GameObjectManager::ExecuteEnemyOrders(std::shared_ptr<GameObject> a_Target, std::vector<std::shared_ptr<CombatUnit>> a_CombatUnitsSelected)
{
		if ((dynamic_cast<Unit*>(a_Target.get()) != nullptr) || (dynamic_cast<Building*>(a_Target.get()) != nullptr))
		{
			for (size_t i = 0; i < a_CombatUnitsSelected.size(); i++)
			{
				a_CombatUnitsSelected[i]->StartAttack(a_Target.get());
			}
		}
}

void GameObjectManager::ExecuteEnemyOrders(std::shared_ptr<GameObject> a_Target, std::vector<std::shared_ptr<BuilderUnit>> a_SelectedBuilders)
{
	if (dynamic_cast<Resource*>(a_Target.get()) != nullptr)
	{
		for (size_t i = 0; i < a_SelectedBuilders.size(); i++)
		{
			a_SelectedBuilders[i]->StartCollect(std::dynamic_pointer_cast<Resource>(a_Target).get());
		}
	}
}


void GameObjectManager::ExecuteEnemyOrders(std::shared_ptr<House> a_House)
{
	a_House->ProduceUnit();
}

void GameObjectManager::ExecuteEnemyOrders(int a_UnitType, std::shared_ptr<Barrack> a_Barrack)
{
	a_Barrack->ProduceUnit(a_UnitType);
}


void GameObjectManager::SetPlayerControls(std::shared_ptr<PlayerControls> a_PlayerControls)
{
	m_PlayerControls = a_PlayerControls;
}

void GameObjectManager::PlaceBuilding(glm::vec2 a_Building, int a_BuildingType, std::shared_ptr<Player> a_Player)
{
	if (a_BuildingType == 1 && a_Player->GetFaction()->GetCurrency() >= HOUSECOST)
	{
		std::shared_ptr<House> NewHouse = std::make_shared<House>(a_Building, shared_from_this(), a_Player);
		m_EntityManager->AddObject(NewHouse);
		
			m_BuilderUnitsSelected->StartBuild(NewHouse, a_BuildingType);
		
		a_Player->GetFaction()->RemoveCurrency(HOUSECOST);
	}
	else if (a_BuildingType == 2 && a_Player->GetFaction()->GetCurrency() >= BARRACKCOST)
	{
		std::shared_ptr<Barrack> NewBarrack = std::make_shared<Barrack>(a_Building,shared_from_this(), a_Player);
		m_EntityManager->AddObject(NewBarrack);
		
			m_BuilderUnitsSelected->StartBuild(NewBarrack, a_BuildingType);
		
		a_Player->GetFaction()->RemoveCurrency(BARRACKCOST);
	}
	else
	{
		printf("wrong type of building or not enough currency to pay the building");
	}

}

void GameObjectManager::PlaceEnemyBuilding(glm::vec2 a_Building, int a_BuildingType, std::shared_ptr<Player> a_Player, std::vector<std::shared_ptr<BuilderUnit>> a_SelectedBuilders)
{
	if (a_BuildingType == 1 && a_Player->GetFaction()->GetCurrency() >= HOUSECOST)
	{
		std::shared_ptr<House> NewHouse = std::make_shared<House>(a_Building, shared_from_this(), a_Player);
		m_EntityManager->AddObject(NewHouse);
		for (size_t i = 0; i < a_SelectedBuilders.size(); i++)
		{
			a_SelectedBuilders[i]->StartBuild(NewHouse, a_BuildingType);
		}
		a_Player->GetFaction()->RemoveCurrency(HOUSECOST);
		a_Player->AddHouse(NewHouse);
	}
	else if (a_BuildingType == 2 && a_Player->GetFaction()->GetCurrency() >= BARRACKCOST)
	{
		std::shared_ptr<Barrack> NewBarrack = std::make_shared<Barrack>(a_Building, shared_from_this(), a_Player);
		m_EntityManager->AddObject(NewBarrack);
		for (size_t i = 0; i < a_SelectedBuilders.size(); i++)
		{
			a_SelectedBuilders[i]->StartBuild(NewBarrack, a_BuildingType);
		}
		a_Player->GetFaction()->RemoveCurrency(BARRACKCOST);
		a_Player->AddBarrack(NewBarrack);
	}
	else
	{
		printf("wrong type of building or not enough currency to pay the building");
	}

}

glm::vec2 GameObjectManager::SearchDeposit(glm::vec2 a_ResourcePosition, std::shared_ptr<Player> a_Player)
{
	float Distance = sqrtf(powf(a_Player->GetCurrentHouse()[0]->GetTransform().Position.x - a_ResourcePosition.x, 2) + powf(a_Player->GetCurrentHouse()[0]->GetTransform().Position.y - a_ResourcePosition.y, 2));
	glm::vec2 ReturnPosition = a_Player->GetCurrentHouse()[0]->GetTransform().Position;
	for (size_t i = 1; i < a_Player->GetCurrentHouse().size(); i++)
	{
		float TempDistance = sqrtf(powf(a_Player->GetCurrentHouse()[i]->GetTransform().Position.x - a_ResourcePosition.x, 2) + powf(a_Player->GetCurrentHouse()[i]->GetTransform().Position.y - a_ResourcePosition.y, 2));
		if (TempDistance < Distance)
		{
			ReturnPosition = a_Player->GetCurrentHouse()[i]->GetTransform().Position;
			Distance = TempDistance;
		}
			
	}

	return ReturnPosition;
}

std::shared_ptr<Resource> GameObjectManager::SearchResourcePosition(std::shared_ptr<Player> a_Player)
{
	float Distance = sqrtf(powf(a_Player->GetCurrentHouse()[0]->GetTransform().Position.x - m_ResourcePoint[0]->GetTransform().Position.x, 2) + powf(a_Player->GetCurrentHouse()[0]->GetTransform().Position.y - m_ResourcePoint[0]->GetTransform().Position.y, 2));
	std::shared_ptr<Resource> ReturnResource = m_ResourcePoint[0];
	for (size_t i = 1; i < m_ResourcePoint.size(); i++)
	{
		float TempDistance = sqrtf(powf(a_Player->GetCurrentHouse()[0]->GetTransform().Position.x - m_ResourcePoint[i]->GetTransform().Position.x, 2) + powf(a_Player->GetCurrentHouse()[0]->GetTransform().Position.y - m_ResourcePoint[i]->GetTransform().Position.y, 2));
		if (TempDistance < Distance)  
		{
			ReturnResource = m_ResourcePoint[i];
			Distance = TempDistance;
		}
	}

	return ReturnResource;
}

std::shared_ptr<Player> GameObjectManager::GetPlayer()
{
	return m_PlayersInGame[0];
}

void GameObjectManager::CreateUnit(glm::vec2 a_UnitPosition, int a_TypeUnit, std::shared_ptr<Player> a_Player)
{
	if (a_TypeUnit == 1)					//create a BuilderUnit
	{
		std::shared_ptr<BuilderUnit> NewBuilderUnit = std::make_shared<BuilderUnit>(m_PathManager, shared_from_this(), a_UnitPosition, a_Player);
		m_EntityManager->AddObject(NewBuilderUnit);
		a_Player->AddBuilderUnit(NewBuilderUnit);
	}
	else if (a_TypeUnit == 2)				//create a MeleeUnit
	{
		std::shared_ptr<MeleeUnit> NewMeleeUnit = std::make_shared<MeleeUnit>(m_PathManager, a_UnitPosition, shared_from_this(), a_Player);
		m_EntityManager->AddObject(NewMeleeUnit);
		a_Player->AddCombatUnit(NewMeleeUnit);
	}
	else if (a_TypeUnit == 3)				//create a RangedUnit
	{
		std::shared_ptr<RangedUnit> NewRangedUnit = std::make_shared<RangedUnit>(m_PathManager, a_UnitPosition, shared_from_this(), a_Player);
		m_EntityManager->AddObject(NewRangedUnit);
		a_Player->AddCombatUnit(NewRangedUnit);
	}
	else
	{
		printf("wrong type of unit or not enough currency to pay the unit");
	}
}

void GameObjectManager::DeleteObject(std::shared_ptr<GameObject> a_GameObject)
{
	m_EntityManager->RemoveObjectElement(a_GameObject);
}

std::vector<std::shared_ptr<GameObject>> GameObjectManager::GetTargetsToAttack(std::shared_ptr<Player> a_Player)
{
	std::vector<std::shared_ptr<GameObject>> ReturnVector;
	for (int i = 0; i < m_PlayersInGame.size(); i++)
	{
		for (int j = 0; j < m_PlayersInGame[i]->GetCurrentCombatUnits().size(); j++)
		{
			ReturnVector.push_back(m_PlayersInGame[i]->GetCurrentCombatUnits()[j]);
		}
		for (int j = 0; j < m_PlayersInGame[i]->GetCurrentHouse().size(); j++)
		{
			ReturnVector.push_back(m_PlayersInGame[i]->GetCurrentHouse()[j]);
		}
		for (int j = 0; j < m_PlayersInGame[i]->GetCurrentBuilderUnits().size(); j++)
		{
			ReturnVector.push_back(m_PlayersInGame[i]->GetCurrentBuilderUnits()[j]);
		}
	}


	return ReturnVector;
}
