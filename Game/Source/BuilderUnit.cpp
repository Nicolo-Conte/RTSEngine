#include "BuilderUnit.h"
#include "GameObjectManager.h"


BuilderUnit::BuilderUnit(std::shared_ptr<PathManager> a_PathManager, std::shared_ptr<GameObjectManager> a_GameObjectManager, glm::vec2 a_Position, std::shared_ptr<Player> a_Player) :
	Unit(a_PathManager, a_GameObjectManager, a_Player),
	m_ResourcesCollected(0),
	m_Resource(nullptr),
	m_HousePosition(glm::vec2(8.f, 8.f)),
	m_Building(false),
	m_Collecting(false),
	m_BuildingTime(0.0f),
	m_BuildingType(0),
	m_CollectingState(CollectingState::Collect),
	m_BuildingTarget(nullptr)
{
	m_Transform.Position = a_Position;
	m_CurrentHealth = BUILDERHEALTH;
	m_Color = ImColor(0, 0, 255);
	m_Name = "Builder Unit";
}

BuilderUnit::BuilderUnit(int a_ResourcesCollected, Resource* a_ResourcePosition, bool a_Building, bool a_Collecting) :
	m_ResourcesCollected(a_ResourcesCollected),
	m_Resource(a_ResourcePosition),
	m_HousePosition(glm::vec2(0, 0)),
	m_Building(a_Building),
	m_Collecting(a_Collecting),
	m_BuildingTime(0.0f),
	m_BuildingType(0),
	m_CollectingState(CollectingState::Collect)
{}

BuilderUnit::~BuilderUnit()
{
}

/// <summary>
/// Update method of the class Builder Unit, based on its Current State performs its various functions
/// </summary>
void BuilderUnit::Update(float a_DeltaTime)
{
	if (m_CurrentHealth <= 0)
	{
		m_GameObjectManager->WarnPlayerControls();
		m_Player->DeleteBuilderUnit(shared_from_this());
		m_GameObjectManager->DeleteObject(shared_from_this());
		return;
	}
	switch (m_CurrentState)
	{
	case InAction:				//The BuilderUnit is in this state if it has been ordered to Build or Collect
	{
		if (m_Building)			//if m_Building is true, The StartBuild method has been called, and the BuilderUnit must execute the Build action
		{
			BuildAction(a_DeltaTime);
		}
		else if(m_Collecting)	//if m_Collecting is true, The StartCollect method has been called, and the BuilderUnit must execute the Collect action
		{
			CollectAction(a_DeltaTime);
		}
		else
		{
			printf("The unit is in the InAction state but it isn't taking any action");
		}
		break;
	}
	case Moving:				//The BuilderUnit is in this state if it has been ordered to Move
	{
		if (FollowPath(a_DeltaTime))	//In the FollowPath state, the Unit will have a target position to reach, will stay in this state until he didn’t reach the target position
		{
			m_CurrentState = Idle;
		}
		break;
	}
	case Idle:
	{

		m_MyTask = "I'm waiting for a task";
		/*In the Idle state, Unit will be stationary in his last position, waiting for a command */
		break;
	}
	default:
		printf("something went wrong, the BuilderUnit is not in one of its three states");
		break;
	}
}

/// <summary>
///  method that prepares a BuilderUnit to collect, specifying the position of the Resource Point
/// </summary>
/// <param name="a_ResourcePosition">position of the resource point</param>
void BuilderUnit::StartCollect(Resource* a_ResourcePosition)
{
	m_Building = false;
	m_Collecting = true;
	m_CurrentState = InAction;
	m_CollectingState = Collect;
	m_Resource = a_ResourcePosition;
	m_HousePosition = m_GameObjectManager->SearchDeposit(m_Resource->GetTransform().Position,m_Player);
	m_TargetPosition = m_Resource->GetTransform().Position;
	BuildPath(m_TargetPosition);
	m_MyTask = "I'm going to collect some resource";
	return;
}

/// <summary>
/// method that prepares a BuilderUnit to build, specifying the position and type of the building
/// </summary>
/// <param name="a_TargetPosition">Position of the building</param>
/// <param name="a_BuildingType">type of the building: 1--> House, 2--> Barrack</param>
void BuilderUnit::StartBuild(std::shared_ptr<Building> a_TargetPosition, int a_BuildingType)
{
	m_Building = true;
	m_Collecting = false;
	m_CurrentState = InAction;
	m_BuildingTarget = a_TargetPosition;
	m_TargetPosition = a_TargetPosition->GetTransform().Position;
	BuildPath(m_TargetPosition);
	m_BuildingType = a_BuildingType;
	m_MyTask = "I'm going to build a Construction";
	if (m_BuildingType == 1)
	{
		m_BuildingTime = HOUSECONSTRUCTIONTIME;
	}
	else if(m_BuildingType == 2)
	{
		m_BuildingTime = BARRACKCONSTRUCTIONTIME;
	}
	else
	{
		printf("wrong type of building");
	}
	return;
}


void BuilderUnit::StartMove(glm::vec2 a_TargetPosition)
{
	m_Building = false;
	m_Collecting = false;
	m_CurrentState = Moving;
	m_TargetPosition = a_TargetPosition;
	BuildPath(m_TargetPosition);
	return;
}
 


/// <summary>
/// This method is called if the Build command has been given. This method takes care of moving the BuilderUnit to the construction site of the building,
/// once reached wait for the construction time, when the time is up it changes the state to Idle
/// <param name="a_DeltaTime"></param>
/// </summary>
void BuilderUnit::BuildAction(float a_DeltaTime)
{

	if (FollowPath(a_DeltaTime))			//call FollowPath function with target the to reach --> Building position. If the builder is already enough close return true and decrease m_BuildingTime
	{										//if not the builder will just move in direction of the Building position

		m_MyTask = "I'm building";
		if (m_BuildingTarget->GetBuildersPresent() <= 0)

		{
			m_BuildingTarget->SetBuildersPresent(1);
		}
		if (m_BuildingTarget->GetCurrentConstructionTime() > 0)
		{
			return;
		//	m_BuildingTime-= a_DeltaTime;
		}
		else								//If the m_BuildingTime arrived to 0, the building is finished and the BuilderUnit switch to the Idle state
		{
			m_BuildingType = 0;
			m_CurrentState = Idle;
			m_Building = false;
			m_BuildingTarget->SetBuildersPresent(0);
			m_BuildingTarget = nullptr;
			m_TargetPosition = glm::vec2(0, 0);
		}
	}
}

float d = 0.0f;

/// <summary>
/// This method is called if the Collect command has been given. This method takes care of moving the BuilderUnit between the resource point,
/// where it will collect the resources, and the nearest House, where it will go to deposit them.
/// </summary>
void BuilderUnit::CollectAction(float a_DeltaTime)
{
	
	switch(m_CollectingState)
		{
		case Collect:
		{
			m_MyTask = "I'm collecting";
			if (m_Resource->GetCurrentAmount() > 0)
			{
				if (FollowPath(a_DeltaTime))
				{
					if (m_ResourcesCollected < 10)		//call FollowPath function with target the to reach --> Resource point. If the builder is already enough close return true and collect 1 resource
					{	
						
							m_ResourcesCollected++;
							m_Resource->AddResources(-1);
						
					}
					else								//if the builder already have collected 10 resources, switch his collecting state to Deposit  
					{									//and set m_HousePosition as the target for the Move function
						m_TargetPosition = m_HousePosition;
						BuildPath(m_TargetPosition);
						m_CollectingState = Deposit;
					}
				}
			}
			else
			{
				if (m_Resource != nullptr)		//check if the Resource point is alredy deleted from the game
				{
					//m_GameObjectManager->DeleteObject((std::shared_ptr<GameObject>)m_Resource);	//probably this cast doesn't work, need test
				}
				m_Resource = nullptr;
				m_CurrentState = Idle;
				m_Collecting = false;
				m_TargetPosition = glm::vec2(0, 0);
			}
				break;
		}
		case Deposit:
		{

			m_MyTask = "I'm depositing the resources collected";
			if (FollowPath(a_DeltaTime))		//call FollowPath function with target the to reach -->House. If the builder is already enough close return true and release the resources and 
			{									//switch his state to collect. If not the builder will just move in direction of the House position
				m_Player->GetFaction()->AddCurrency(m_ResourcesCollected);
				m_ResourcesCollected = 0;
				m_TargetPosition = m_Resource->GetTransform().Position;
				BuildPath(m_TargetPosition);
				m_CollectingState = Collect;
			}
			break;
		}
		default:
			printf("something went wrong");
		break;
	}
}

/// <summary>
/// function used by the units to reach their current target position
/// </summary>
/// <returns>return true if the unit have reached the target, else return false</returns>
bool BuilderUnit::Move(float a_DeltaTime)
{
	glm::vec2 Direction = m_TargetPosition - m_Transform.Position;
	float length = sqrt((Direction.x * Direction.x) + (Direction.y * Direction.y));
	Direction = Direction / length;
	a_DeltaTime;
	float Distance = sqrtf(powf(m_TargetPosition.x - m_Transform.Position.x, 2) + powf(m_TargetPosition.y - m_Transform.Position.y, 2));
	m_Transform.Position = m_Transform.Position + (Direction * BUILDERSPEED);

	if (Distance > 20.0f)		//Check if the unit is close enough to its target,  8.0f-->temporary value to change, 
	{
		return false;
	}
	else
	{
		return true;
	}

}