#include "Building.h"
#include "Player.h"
Building::Building() :
	m_CurrentState(BuildingState::InConstruction),
	m_BuildersPresent(0),
	m_CurrentUnitTime(0.0f)
{}

Building::Building(std::shared_ptr<GameObjectManager> a_GameObjectManager, std::shared_ptr<Player> a_Player) :
	GameObject(a_GameObjectManager, a_Player),
	m_CurrentState(BuildingState::InConstruction),
	m_BuildersPresent(0),
	m_CurrentUnitTime(0.0f)
{}

Building::~Building()
{
}

/// <summary>
/// The method checks if the there are at least one BuilderUnit who are constructing and if the construction time is finished or not
/// </summary>
/// <returns>If the CurrentConstructionTime is more than 0 return false, else if return true</returns>
bool Building::CheckConstruction(float a_DeltaTime)
{
	if (m_CurrentConstructionTime > 0.0f) {		//if the ConstructionTime isn't finshed yet, check if there are at least one BuilderUnit
		if (m_BuildersPresent >= 1)				//if there are at least one BuilderUnit, decrease ConstructionTime 
		{
			printf("a BuilderUnit is currently constructing it\n");
			m_CurrentConstructionTime-= a_DeltaTime;
			return false;
		}
		else									//if there aren't BuilderUnit, stop decrease ConstructionTime
		{
			printf("Building under construction but no one BuilderUnit is currently constructing it\n");
			return false;
		}
	}
	else										//if the ConstructionTime is finshed, the building is built and switch to the Inactive state
	{
		printf("the building was built");
		m_CurrentState = Inactive;
		m_BuildersPresent = 0;
		return true;
	}
}



