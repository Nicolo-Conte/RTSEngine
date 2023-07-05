#pragma once
#include "GameObject.h"
#include "PathManager.h"

/// <summary>
/// represents the base class of the different types of troops.                        
/// </summary>
class Unit : public GameObject
{
public:

enum UnitState	//The three different state which a Unit object can be. 
	{
		Idle = 0,	//In the Idle state, Unit will be stationary in his last position, waiting for a command
		Moving,		//In the Moving state, the Unit will have a target position to reach, will stay in this state until the object is in moving and didn’t reach the position
		InAction	//In the In_Action State, the Unit have a specific task to complete ( for Attacker Unit will be Attack action, for builder unit will be Build action
	};
	//Constructors & Destructor
	Unit();
	//Base constructor for inheritance
	Unit(std::shared_ptr<PathManager> a_PathManager, std::shared_ptr<GameObjectManager> m_GameObjectManager, std::shared_ptr<Player> a_Player);
	~Unit();

	//Setter & Getter
	glm::vec2 GetTargetPosition();
	void SetTargetPosition(glm::vec2 a_TargetPosition);

	std::vector<glm::vec2> GetQueuePathTarget();
	void SetQueuePathTarget(std::vector<glm::vec2> a_QueuePathTarget);

	std::shared_ptr<PathManager> GetPathManager();
	void SetPathManager(std::shared_ptr<PathManager> a_QueuePathTarget);



	//other functions
	void BuildPath(glm::vec2 a_TargetPosition);
	bool FollowPath(float a_DeltaTime);
	virtual bool Move(float a_DeltaTime) = 0;
	void StartMove(glm::vec2 a_TargetPosition);

protected:



	UnitState m_CurrentState;					//Handles the the current state of a Unit class
	glm::vec2 m_TargetPosition;					//Assign to the Unit a specific Position to reach and Switch the state of a Unit to Moving state;
	std::vector<glm::vec2> m_QueuePathTarget;	//Queue that contains the various positions to follow to reach the TargetPosition
	std::shared_ptr<PathManager> m_PathManager;	//member class used to build the Path to follow
	std::shared_ptr<Node> m_CurrentNode;
};

inline glm::vec2 Unit::GetTargetPosition()
{
	return m_TargetPosition;
}

inline void Unit::SetTargetPosition(glm::vec2 a_TargetPosition)
{
	m_TargetPosition = a_TargetPosition;
}

inline std::vector<glm::vec2> Unit::GetQueuePathTarget()
{
	return m_QueuePathTarget;
}

inline void Unit::SetQueuePathTarget(std::vector<glm::vec2> a_QueuePathTarget)
{
	m_QueuePathTarget = a_QueuePathTarget;
}

inline std::shared_ptr<PathManager> Unit::GetPathManager()
{
	return m_PathManager;
}

inline void Unit::SetPathManager(std::shared_ptr<PathManager> a_PathManager)
{
	m_PathManager = a_PathManager;
}
