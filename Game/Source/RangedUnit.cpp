#include "RangedUnit.h"
#include "GameObjectManager.h"
RangedUnit::RangedUnit(std::shared_ptr<PathManager> a_PathManager, glm::vec2 a_Position, std::shared_ptr<GameObjectManager> a_GameObjectManager, std::shared_ptr<Player> a_Player) :
	CombatUnit(a_PathManager, a_GameObjectManager, a_Player)
{
	m_Transform.Position = a_Position;
	m_CurrentHealth = RANGEDHEALTH;
	m_Color = ImColor(0, 255, 0);
	m_Name = "Ranged Unit";
}

RangedUnit::~RangedUnit()
{
}


/// <summary>
/// 
/// </summary>
/// <returns></returns>
bool RangedUnit::CheckDistance()
{
	float Distance = sqrtf(powf(m_TargetEnemy->GetTransform().Position.x - m_Transform.Position.x, 2) +
						   powf(m_TargetEnemy->GetTransform().Position.y - m_Transform.Position.y, 2));

	if (Distance < RANGEDDISTANCE * 8 && m_TargetEnemy->GetUnderAttack() == false)
	{
		m_TargetEnemy->SetUnderAttack(true);
	}
	if (Distance < RANGEDDISTANCE)
	{
		return true;
	}
	else
	{
		return false;
	}


}


/// <summary>
/// 
/// </summary>
void RangedUnit::AttackAction(float a_DeltaTime)
{
	
	if (CheckDistance())
	{	
		m_QueuePathTarget.clear();
		if ( m_TargetEnemy->GetCurrentHealth() > 0)
		{	
			m_TargetEnemy->SetCurrentHealth(m_TargetEnemy->GetCurrentHealth() - RANGEDATTACK);
		}
		else
		{
			
			m_CurrentState = Idle;
			m_TargetEnemy = nullptr;
			m_UnderAttack = false;
		}
	}
	else
	{
		if (m_QueuePathTarget.size() == 0)
		{
			m_TargetPosition = m_TargetEnemy->GetTransform().Position;
			Move(a_DeltaTime);
		}
		else
			FollowPath(a_DeltaTime);
	}
	
}

/// <summary>
/// 
/// </summary>
/// <param name="a_DeltaTime"></param>
void RangedUnit::Update(float a_DeltaTime)
{
	if (m_CurrentHealth <= 0)
	{
		m_GameObjectManager->WarnPlayerControls();
		m_Player->DeleteCombatUnit(shared_from_this());
		m_GameObjectManager->DeleteObject(shared_from_this());
		return;
	}
	switch (m_CurrentState)
	{
	case InAction:				//The RangedUnit is in this state if it has been ordered to Attack
	{
		AttackAction(a_DeltaTime);
		break;
	}
	case Moving:				//The RangedUnit is in this state if it has been ordered to Move
	{
		if (m_UnderAttack)		//if an enemy starts to attack a RangedUnit during Idle state, switch its state to InAction and call AttackPreparation()
		{
			AttackPreparation();
			break;
		}
		if (FollowPath(a_DeltaTime))	//In the FollowPath state, the Unit will have a target position to reach, will stay in this state until he didn’t reach the target position
		{
			m_CurrentState = Idle;
		}
		break;
	}
	case Idle:
	{

		m_MyTask = "I'm waiting for a task";
		if (m_UnderAttack)		//if an enemy starts to attack a RangedUnit during Idle state, switch its state to InAction and call AttackPreparation()
		{
			AttackPreparation();
		}
		/*In the Idle state, Unit will be stationary in his last position, waiting for a command */
		break;
	}
	default:
		printf("something went wrong, the RangedUnit is not in one of its three states");
		break;
	}
}

/// <summary>
/// function used by the units to reach their current target position
/// </summary>
/// <returns>return true if the unit have reached the target, else return false</returns>
bool RangedUnit::Move(float a_DeltaTime)
{
	a_DeltaTime;
	glm::vec2 Direction = m_TargetPosition - m_Transform.Position;
	float length = sqrt((Direction.x * Direction.x) + (Direction.y * Direction.y));
	Direction = Direction / length;
	float Distance = sqrtf(powf(m_TargetPosition.x - m_Transform.Position.x, 2) + powf(m_TargetPosition.y - m_Transform.Position.y, 2));
	m_Transform.Position = m_Transform.Position + (Direction  *RANGEDSPEED);

	if (Distance > 8.0f)		//Check if the unit is close enough to its target,  2.0f-->temporary value to change, 
	{
		return false;
	}
	else
	{
		return true;
	}

}