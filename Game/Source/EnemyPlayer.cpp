#include "EnemyPlayer.h"
#include "GameObjectManager.h"

EnemyPlayer::EnemyPlayer()
{
}

EnemyPlayer::EnemyPlayer(std::shared_ptr<Faction> a_Faction) :
	Player(a_Faction),
	m_CurrentState(GatheringResources),
	m_PreviousState(HouseBuilding),
	m_ResourceNeeded(HOUSECOST),
	m_HousesNeeded(3),
	m_BarrackNeeded(1),
	m_NumBuilderNeeded(STARTBUILDERUNITS)
{
	CurrentAction.push_back("I'm going to gather resources");
}

EnemyPlayer::~EnemyPlayer()
{
}


/// <summary>
/// 
/// </summary>
/// <param name="a_DeltaTime"></param>
void EnemyPlayer::Update(float a_DeltaTime)
{
	///*
	//* increase the number of builders needed depending on the time since the start of the match
	//*/
	a_DeltaTime = 0;
	m_CurrentState = ChangeState();
	switch (m_CurrentState)
	{
	case HouseBuilding:
	{
		srand((unsigned int)time(NULL));
		int randx = rand() % 15 + 1;
		int randy = rand() % 15 + 1;
		if (m_Faction->GetCurrency() >= HOUSECOST)
		{
			m_SelectedBuilders.push_back(CheckForBuilders());		//Search for Builder Unit in Idle State or Collect State
			if(m_SelectedBuilders.size()>0)
			{ 
				m_SelectedBuilders[0]->GetGameObjectManager()->PlaceEnemyBuilding(glm::vec2(m_CurrentHouse[0]->GetTransform().Position.x + randx *16,
					m_CurrentHouse[0]->GetTransform().Position.x + randy *16), 1, shared_from_this(), m_SelectedBuilders);	//implement a method to search a free place
				m_SelectedBuilders.clear();
			}
		}
		else
		{
			m_PreviousState = HouseBuilding;
			m_ResourceNeeded = HOUSECOST - m_Faction->GetCurrency();
			m_CurrentState = GatheringResources;
		}
		break;
	}
	case BarrackBuilding:
	{
		
		if (m_Faction->GetCurrency() >= BARRACKCOST)
		{
			srand((unsigned int)time(NULL));
			int randx = rand() % 15 + 1;
			int randy = rand() % 15 + 1;
			m_SelectedBuilders.push_back(CheckForBuilders());		//Search for Builder Unit in Idle State or Collect State
			m_SelectedBuilders[0]->GetGameObjectManager()->PlaceEnemyBuilding(glm::vec2(m_CurrentHouse[0]->GetTransform().Position.x + randx*16,
																				   m_CurrentHouse[0]->GetTransform().Position.x + randy*16),
																				   2, shared_from_this(), m_SelectedBuilders);	//implement a method to search a free place
			m_SelectedBuilders.clear();

		}
		else
		{
			m_PreviousState = BarrackBuilding;
			m_ResourceNeeded = BARRACKCOST - m_Faction->GetCurrency();
			m_CurrentState = GatheringResources;
		}
		
		break;
	}
	case GatheringResources:
	{
		if (m_Faction->GetCurrency() >= (unsigned)m_ResourceNeeded)
		{
			m_CurrentState = m_PreviousState;
		}
		if (CheckFreeBuilders().size() >= 1)
		{

			m_SelectedBuilders = CheckFreeBuilders() ;		//Search for Builder Unit in Idle State or Collect State
			if(m_SelectedBuilders.size()>0)
			{
				m_SelectedBuilders[0]->GetGameObjectManager()->ExecuteEnemyOrders(m_NearResourcesPoint[0], m_SelectedBuilders);	//implement a method to search the nearest ResourcePoint
				m_SelectedBuilders.clear();
			}
		}
		else if(m_CurrentBuilderUnits.size() < 1)
		{
			m_CurrentState = ProducingBuilders;
		}
		break;
	}
	case BuildingArmy:
	{
		if (m_CurrentBarrack.size() > 0)
		{
			srand((unsigned int)time(NULL));
			int combatUnit = rand() % 2 + 1;
			int costUnit = (combatUnit == 1) ? RANGEDCOST : MELEECOST;
			if (m_Faction->GetCurrency() >= (unsigned)costUnit)
			{
				if (m_CurrentPopulation == m_MaxPopulation && m_HouseUnderConstruction.size() == 0)
				{
					m_CurrentState = HouseBuilding;
					m_HousesNeeded++;
					return;
				}
				//call produceUnit(combatUnit) [GameObjectManager]
				m_CurrentBarrack[0]->GetGameObjectManager()->ExecuteEnemyOrders(combatUnit, m_CurrentBarrack[0]);
			}
			else
			{
				m_PreviousState = BuildingArmy;
				m_ResourceNeeded = costUnit - m_Faction->GetCurrency();
				m_CurrentState = GatheringResources;
			}
		}
		break;
	}
	case Attack:
	{
		if (m_CurrentCombatUnits.size() < 3)
		{
			//orders the remaining troops to retreat
			m_CurrentState = BuildingArmy;
			return;
		}
		else
		{
			if (m_PossibleTargetsToAttack.size() > 0)			//use m_PossibileTargetToAttack to get enemiesTarget and oreder to the combat unit to attack
			{
				m_CurrentCombatUnits[0]->GetGameObjectManager()->ExecuteEnemyOrders(m_PossibleTargetsToAttack[0], m_CurrentCombatUnits);
				m_PossibleTargetsToAttack.erase(m_PossibleTargetsToAttack.begin());
			}
			
		}
		break;
	}
	case Defense:
	{
		if(m_BuildingUnderAttack == true)
		{  
			if (m_CurrentCombatUnits.size() < 1)
			{
				srand((unsigned int)time(NULL));
				int combatUnit = rand() % 2 + 1;
				m_CurrentBarrack[0]->GetGameObjectManager()->ExecuteEnemyOrders(combatUnit, m_CurrentBarrack[0]);
			}
			else
			{
				m_CurrentCombatUnits[0]->GetGameObjectManager()->ExecuteEnemyOrders(m_PossibleTargetsToAttack[0], m_CurrentCombatUnits);
				m_PossibleTargetsToAttack.erase(m_PossibleTargetsToAttack.begin());
			}
		}
		else
		{
			m_CurrentState = BuildingArmy;
			return;
		}
		break;
	}
	case ProducingBuilders:
	{
		if (m_CurrentHouse.size() >= 1) {
			if (static_cast<int>(m_CurrentBuilderUnits.size()) < m_NumBuilderNeeded)
			{
				if (m_Faction->GetCurrency() >= BUILDERCOST)
				{
					if (m_CurrentPopulation == m_MaxPopulation && m_HouseUnderConstruction.size() == 0)
					{
						m_CurrentState = HouseBuilding;
						m_HousesNeeded++;
					}
					m_CurrentHouse[0]->GetGameObjectManager()->ExecuteEnemyOrders(m_CurrentHouse[0]);
				}
				else
				{
					if (m_CurrentBuilderUnits.size() == 0)
						printf("the player have no more builder units no resource to produce one. The player lose");
					else
					{
						m_PreviousState = ProducingBuilders;
						m_ResourceNeeded = BUILDERCOST - m_Faction->GetCurrency();
						m_CurrentState = GatheringResources;
					}
				}
			}
			else
			{
				m_CurrentState = ChangeState();
			}
		}
		else
		{
			m_CurrentState = HouseBuilding;
		}
		break;
	}
	case Lose:
	{
		printf("I lose the match");
	}
	default:
		printf("The enemy player isn't in one of its States");
		break;
	}
}

EnemyPlayer::EnemyState EnemyPlayer::ChangeState()
{
	if (CheckforLose() != true) {
		if (m_BuildingUnderAttack == true && m_CurrentState != Defense)
		{
			if (m_CurrentCombatUnits.size() < 1)
			{
				if (CurrentAction.back() != "I doesn't have Combat unit for defense, I'm going to produce some combat unit")
				CurrentAction.push_back("I doesn't have Combat unit for defense, I'm going to produce some combat unit");
				return BuildingArmy;
			}
			else
			{
				if (CurrentAction.back() != "I'm under attack, now I'm going to defend")
				CurrentAction.push_back("I'm under attack, now I'm going to defend");
				m_PossibleTargetsToAttack = m_CurrentCombatUnits[0]->GetGameObjectManager()->GetTargetsToAttack(shared_from_this());
				return Defense;
			}
		}
		if (static_cast<int>(m_CurrentBuilderUnits.size()) < m_NumBuilderNeeded)
		{
			if (CurrentAction.back() != "I'm going to produce more builders unit")
			CurrentAction.push_back("I'm going to produce more builders unit");
			return ProducingBuilders;
		}
		if (m_CurrentState != GatheringResources)
		{
			if (static_cast<int>(m_CurrentHouse.size()) < m_HousesNeeded)
			{
				if (CurrentAction.back() != "I'm going to build more house")
				CurrentAction.push_back("I'm going to build more house");
				return HouseBuilding;
			}
			if (static_cast<int>(m_CurrentHouse.size()) >= m_HousesNeeded && static_cast<int>(m_CurrentBarrack.size()) < m_BarrackNeeded)
			{
				if (CurrentAction.back() != "I'm going to build a Barrack")
				CurrentAction.push_back("I'm going to build a Barrack");
				return BarrackBuilding;
			}
			if (static_cast<int>(m_CurrentHouse.size()) >= m_HousesNeeded && static_cast<int>(m_CurrentBarrack.size()) >= m_BarrackNeeded && m_CurrentCombatUnits.size() < 10)
			{
				if (CurrentAction.back() != "I'm going to build a my army")
				CurrentAction.push_back("I'm going to build a my army");
				return BuildingArmy;
			}
			if (static_cast<int>(m_CurrentHouse.size()) >= m_HousesNeeded && static_cast<int>(m_CurrentBarrack.size()) >= m_BarrackNeeded && m_CurrentCombatUnits.size() >= 10)
			{
				if (CurrentAction.back() != "I'm going to attack the enemy")
				CurrentAction.push_back("I'm going to attack the enemy");
				m_PossibleTargetsToAttack = m_CurrentCombatUnits[0]->GetGameObjectManager()->GetTargetsToAttack(shared_from_this());
				return Attack;
			}
		}
		if(CurrentAction.back() != "I'm going to gather resources")
		CurrentAction.push_back("I'm going to gather resources");
		return GatheringResources;		//probably to change with PreviousState
	}
	{
		return Lose;
	}
}

std::shared_ptr<BuilderUnit> EnemyPlayer::CheckForBuilders()
{

	for (size_t i = 0; i < m_CurrentBuilderUnits.size(); i++)
	{
		if (m_CurrentBuilderUnits[i]->GetState() == Unit::UnitState::Idle || (m_CurrentBuilderUnits[i]->GetState() == Unit::UnitState::InAction && m_CurrentBuilderUnits[i]->GetCollecting()) )
			return m_CurrentBuilderUnits[i] ;
	}
	return nullptr;
}


std::vector<std::shared_ptr<BuilderUnit>> EnemyPlayer::CheckFreeBuilders()
{
	std::vector<std::shared_ptr<BuilderUnit>> ReturnBuilders;
	for (size_t i = 0; i < m_CurrentBuilderUnits.size(); i++)
	{
		if (m_CurrentBuilderUnits[i]->GetState() == Unit::UnitState::Idle)
			ReturnBuilders.push_back(m_CurrentBuilderUnits[i]);
	}
	return ReturnBuilders;
}

bool EnemyPlayer::CheckforLose()
{
	if (m_CurrentHouse.size() < 0 && m_CurrentBuilderUnits.size() < 0)
	{
		m_LoseMatch = true;
	}

	return m_LoseMatch;
}




