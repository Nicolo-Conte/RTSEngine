#include "Barrack.h"
#include "GameObjectManager.h"

Barrack::Barrack(glm::vec2 a_Position, std::shared_ptr<GameObjectManager> a_GameObjectManager, std::shared_ptr<Player> a_Player) :
	Building(a_GameObjectManager, a_Player)
{
	m_CurrentConstructionTime = BARRACKCONSTRUCTIONTIME;
	m_Transform.Position = a_Position;
	m_CurrentState = InConstruction;
	m_CurrentHealth = BARRACKHEALTH;
	m_Player = a_Player;
	m_Size = 2.0f;
	m_Color = ImColor(255, 0, 0);
	m_Name = "Barrack";
}

Barrack::~Barrack()
{}


/// <summary>
/// function that increases the production queue of the Barrack by inserting the right type of CombatUnit, if the Barrack is in the state of Inactive switch it on Active
/// </summary>
void Barrack::ProduceUnit(int a_UnitType)
{
	if ((a_UnitType == 1 && m_Player->GetFaction()->GetCurrency() >= MELEECOST) || (a_UnitType == 2 && m_Player->GetFaction()->GetCurrency() >= RANGEDCOST))
	{
		if (m_CurrentState == Inactive)											//check if the Barrack is currently producing CombatUnits or not
		{
			m_UnitQueue.push_back(a_UnitType);
			if (a_UnitType == 1)
			{
				m_CurrentUnitTime = MELEECONSTRUCTIONTIME;
				m_Player->GetFaction()->RemoveCurrency(MELEECOST);
			}
			else
			{
				m_CurrentUnitTime = RANGEDCONSTRUCTIONTIME;
				m_Player->GetFaction()->RemoveCurrency(RANGEDCOST);
			}
			m_CurrentState = Active;
		}
		else if (m_CurrentState == InConstruction)
		{
			printf("The Barrack is under construction, cannot produce units");
		}
		else																	//if is already producing units, add the unit to the production queue 
		{
			if (a_UnitType == 1)
				m_Player->GetFaction()->RemoveCurrency(MELEECOST);
			else
				m_Player->GetFaction()->RemoveCurrency(RANGEDCOST);
			m_UnitQueue.push_back(a_UnitType);
		}
	}
}

/// <summary>
/// function that decreases the production time of each unit;
/// manages the production queue;
/// if the Barrack has finished producing units switch the status to Inactive
/// <param name="a_DeltaTime"></param>
/// </summary>
void Barrack::ProduceAction(float  a_DeltaTime)
{

	m_MyTask = "I'm producing Combat Units";
	if (m_CurrentUnitTime > 0.0f)												//checks if the production time of the current unit is over, and if it isn't, decrements it
	{
		m_CurrentUnitTime-= a_DeltaTime;
	}
	else																	//decrease Unit queue and check if there are other units to produce
	{
		
		srand((unsigned int)time(NULL));
		int randx = rand() % 5 + 1;
		int randy = rand() % 5 + 1;
		if (m_UnitQueue.front() == 1)
		{
			m_GameObjectManager->CreateUnit(glm::vec2(m_Transform.Position.x + randx*16, m_Transform.Position.y + randy * 16),2,m_Player);
		}
		else
		{
			m_GameObjectManager->CreateUnit(glm::vec2(m_Transform.Position.x + randx*16, m_Transform.Position.y + randy * 16),3, m_Player);
		}

		m_UnitQueue.erase(m_UnitQueue.begin());
		if (m_UnitQueue.size() > 0)
		{
			if (m_UnitQueue.front() == 1)
			{
				m_CurrentUnitTime = MELEECONSTRUCTIONTIME;
			}
			else
			{
				m_CurrentUnitTime = RANGEDCONSTRUCTIONTIME;
			}
		}
		else																//if there aren't other untis to produce, switch the state to Inactive
		{
			m_CurrentUnitTime = 0.0f;
			m_CurrentState = Inactive;
		}
	}
}

/// <summary>
/// Update method of the class Barrack, based on its Current State performs its various functions
/// <param name="a_DeltaTime"></param>
/// </summary>
void Barrack::Update(float a_DeltaTime)
{
	if (m_CurrentHealth <= 0)
	{
		m_GameObjectManager->WarnPlayerControls();
		m_Player->DeleteBarrack(shared_from_this());
		m_GameObjectManager->DeleteObject(shared_from_this());
		return;
	}
	if (m_UnderAttack)
	{
		m_Player->SetBuildingUnderAttack(true);
	}
	switch (m_CurrentState)
	{
	case InConstruction:				//If The Barrack is in this state is under construction and can only wait for the end
	{

		m_MyTask = "I'm under construction";
		CheckConstruction(a_DeltaTime);			//The CheckConstruction will switch the state of the Barrack(from InConstruction to Inactive) when m_CurrentConstructionTime is arrived to 0

		break;
	}
	case Active:						//In the Active state, the Barrack is producing CombatUnits
	{
		ProduceAction(a_DeltaTime);
		break;
	}
	case Inactive:						//In the Inactive state, isn't producing units and wait for a command
	{

		m_MyTask = "I'm waiting for a task";
		break;
	}
	default:
		printf("something went wrong, the Barrack is not in one of its three states");
		break;
	}

}
