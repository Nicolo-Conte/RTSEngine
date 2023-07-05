#pragma once
#include "Player.h"

class GameObjectManager;

class EnemyPlayer : public Player, public std::enable_shared_from_this<EnemyPlayer>
{
public:

	enum EnemyState
	{
		HouseBuilding = 0,
		BarrackBuilding,
		GatheringResources,
		BuildingArmy,
		Attack,
		Defense,
		ProducingBuilders,
		Lose
	};

	//Constructors & Destructor
	EnemyPlayer();
	EnemyPlayer(std::shared_ptr<Faction> a_Faction);
	~EnemyPlayer();

	//getter and setter
	void SetResourcePoint(std::shared_ptr<Resource>);

	//other methods
	void Update(float a_DeltaTime);
	EnemyState ChangeState();
	std::shared_ptr<BuilderUnit> CheckForBuilders();
	std::vector<std::shared_ptr<BuilderUnit>> CheckFreeBuilders();
	bool CheckforLose();
	std::vector < std::string> GetAction() { return CurrentAction; }
private:

	EnemyState m_CurrentState;
	EnemyState m_PreviousState;
	int m_ResourceNeeded;
	int m_HousesNeeded;
	int m_BarrackNeeded;
	int m_NumBuilderNeeded;		//number of builders that the player need to afford the resources demand
	//bool m_IncreaseMaxPopulation;
	std::vector<std::shared_ptr<GameObject>> m_PossibleTargetsToAttack;
	std::vector<std::shared_ptr<BuilderUnit>> m_SelectedBuilders;
	std::vector<std::shared_ptr<Resource>> m_NearResourcesPoint;
	std::vector < std::string> CurrentAction;
};

inline void EnemyPlayer::SetResourcePoint(std::shared_ptr<Resource> a_Resource)
{
	m_NearResourcesPoint.push_back(a_Resource);
}