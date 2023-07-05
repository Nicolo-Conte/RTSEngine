#include "House.h"
#include "GameObjectManager.h"

House::House(glm::vec2 a_Position, std::shared_ptr<GameObjectManager> a_GameObjectManager, std::shared_ptr<Player> a_Player) :
	Building(a_GameObjectManager, a_Player),
	m_UnitQueue(0)
{
	m_CurrentConstructionTime = HOUSECONSTRUCTIONTIME;	
	a_Player->AddHouseUnderConstruction(true);
	m_Transform.Position = a_Position;
	m_CurrentState = InConstruction;
	m_CurrentHealth = HOUSEHEALTH;
	m_Size = 1.5f;
	m_Color = ImColor(255, 255, 0);
	m_Name = "House";
}

House::~House()
{}

/// <summary>
/// function that increases the production queue of the House, if the house is in the state of Inactive switch it on Active
/// </summary>
void House::ProduceUnit() //a_UnitType
{

	m_MyTask = "I'm producing Builder Unit";
	if (m_Player->GetFaction()->GetCurrency() >= BUILDERCOST)
	{
		if (m_CurrentState == Inactive)											//check if the House is currently producing BuilderUnits or not
		{
			m_CurrentUnitTime = BUILDERCONSTRUCTIONTIME;
			m_UnitQueue++;
			m_CurrentState = Active;
			m_Player->GetFaction()->RemoveCurrency(BUILDERCOST);
		}
		else if (m_CurrentState == InConstruction)
		{
			printf("The House is under construction, cannot produce units");
		}
		else																	//if is already producing units, only increase the production queue 
		{
			m_UnitQueue++;
			m_Player->GetFaction()->RemoveCurrency(BUILDERCOST);
		}
	}
}

/// <summary>
/// function that decreases the production time of each unit;
/// manages the production queue;
/// if the House has finished producing units switch the status to Inactive
/// <param name="a_DeltaTime"></param>
/// </summary>
void House::ProduceAction(float a_DeltaTime)
{
	if (m_CurrentUnitTime > 0.0f)												//checks if the production time of the current unit is over, and if it isn't, decrements it
	{
		m_CurrentUnitTime-= a_DeltaTime;
	}
	else																	//decrease Unit queue and check if there are other units to produce
	{
		srand((unsigned int)time(NULL));
		int randx = rand() % 5 + 1;
		int randy = rand() % 5 + 1;
		m_UnitQueue--;
		m_GameObjectManager->CreateUnit(glm::vec2(m_Transform.Position.x+ randx * 16, m_Transform.Position.y + randy * 16), 1, m_Player);
		if (m_UnitQueue > 0)
		{
			m_CurrentUnitTime = BUILDERCONSTRUCTIONTIME;
		}
		else																//if there aren't other untis to produce, switch the state to Inactive
		{
			m_CurrentUnitTime = 0.0f;
			m_CurrentState = Inactive;
		}
	}
}

/// <summary>
/// Update method of the class House, based on its Current State performs its various functions
/// <param name="a_DeltaTime"></param>
/// </summary>
void House::Update(float a_DeltaTime)
{
	if (m_CurrentHealth <= 0)
	{
		m_GameObjectManager->WarnPlayerControls();
		m_Player->DeleteHouse(shared_from_this());
		m_GameObjectManager->DeleteObject(shared_from_this());
		return;
	}
	if (m_UnderAttack)
	{
		m_Player->SetBuildingUnderAttack(true);
	}
	switch (m_CurrentState)
	{
	case InConstruction:				//If The House is in this state is under construction and can only wait for the end
	{

		m_MyTask = "I'm under construction";
		CheckConstruction(a_DeltaTime);		//The CheckConstruction will switch the state of the House(from InConstruction to Inactive) when m_CurrentConstructionTime is arrived to 0
		
		break;
	}
	case Active:						//In the Active state, the House is producing BuilderUnit
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
		printf("something went wrong, the House is not in one of its three states");
		break;
	}
}


void House::Build()
{
	m_CurrentConstructionTime = 0;
	m_CurrentState = Inactive;
}
