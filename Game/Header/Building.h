#pragma once
#include <iostream>
#include "GameObject.h"

/// <summary>
/// represents the base class of the different types of constructions.  
/// </summary>
class Building : public GameObject
{
protected:

	enum BuildingState	//The three different state which a Building object can be. 
	{
		InConstruction = 0,	//In the InConstruction state, the Building is under construction and can only wait for the end
		Active,				//In the Active state, the Building is producing units
		Inactive			//In the Inactive state, isn't producing units and wait for a command
	};

	BuildingState m_CurrentState;		//Handles the the current state of a Unit class
	
	int m_BuildersPresent;				//current number of builders who are constructing the building
	float m_CurrentUnitTime;				//remaining time for the production of the current unit
	
public:
	//Constructors & Destructor
	Building();
	Building(std::shared_ptr<GameObjectManager> a_GameObjectManager, std::shared_ptr<Player> a_Player);
	~Building();

	//Setter & Getter
	
	int GetBuildersPresent();
	float GetCurrentUnitTime();
	void SetBuildersPresent(int a_BuildersPresent);
	void SetCurrentUnitTime(float a_CurrentUnitTime);

	//other functions
	bool CheckConstruction(float a_DeltaTime);
	virtual void ProduceUnit() = 0;
	virtual void ProduceAction(float a_DeltaTime) = 0;
};

inline int Building::GetBuildersPresent()
{
	return m_BuildersPresent;
}

inline float Building::GetCurrentUnitTime()
{
	return m_CurrentUnitTime;
}

inline void Building::SetBuildersPresent(int a_BuildersPresent)
{
	m_BuildersPresent = a_BuildersPresent;
}

inline void Building::SetCurrentUnitTime(float a_CurrentUnitTime)
{
	m_CurrentUnitTime = a_CurrentUnitTime;
}