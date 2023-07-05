#include "Unit.h"

Unit::Unit() :
	m_CurrentState(UnitState::Idle),
	m_TargetPosition(glm::vec2(0, 0)),
	m_QueuePathTarget(0),
	m_PathManager(nullptr),
	m_CurrentNode(nullptr)
{}

Unit::Unit(std::shared_ptr<PathManager> a_PathManager, std::shared_ptr<GameObjectManager> a_GameObjectManager, std::shared_ptr<Player> a_Player) :
	GameObject(a_GameObjectManager, a_Player),
	m_CurrentState(UnitState::Idle),
	m_TargetPosition(glm::vec2(0, 0)),
	m_QueuePathTarget(0),
	m_PathManager(a_PathManager),
	m_CurrentNode(nullptr)
{
	m_Size = 1.0f;
}

Unit::~Unit()
{}

/// <summary>
/// PathFinding algorithm, build a path that the unit will have to follow to reach its Target
/// </summary>
/// <param name="a_TargetPosition">Target which a unit have to reach</param>
void Unit::BuildPath(glm::vec2 a_TargetPoisition) //a_TargetPoisition
{
	m_QueuePathTarget = m_PathManager->SolveAStar(m_Transform.Position, a_TargetPoisition);
}

	
/// <summary>
/// method that allows the unit to follow the path composed of nodes (map cells) to reach its target
/// </summary>
/// <param name="a_DeltaTime"></param>
/// <returns>retunr true if The unit doesn't have new position to reach, meaning he's arrived to the target destination</returns>
bool Unit::FollowPath(float a_DeltaTime)
{

	if (Move(a_DeltaTime))
	{
		
		if (m_QueuePathTarget.size() > 0)
		{
			m_QueuePathTarget.erase(m_QueuePathTarget.begin());

			if (m_QueuePathTarget.size() > 0)
			{
				////Rebuild path if the next node became an obstacle during pathing
				//if (m_PathManager->GetNode(m_QueuePathTarget.front())->GetObstacle())
				//{
				//	BuildPath(m_QueuePathTarget.back());
				//}
				//else
				//{
				//	m_TargetPosition = m_QueuePathTarget.front();
				//	return false;
				//}
			}
		}
		else
		{
			
			return true;
		}
	}
	else
	{
		return false;
	}
	return false;
}




void Unit::StartMove(glm::vec2 a_TargetPosition)
{
	m_CurrentState = Moving;
	m_TargetPosition = a_TargetPosition;
	BuildPath(m_TargetPosition);
	return;
}