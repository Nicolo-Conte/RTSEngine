#include "EnemyPlayer.h"
#include "PathManager.h"
#include "GameObjectManager.h"
#include "EntityManager/Camera.h"

#pragma once
class GameMatch
{
public:
	GameMatch();
	~GameMatch();

	std::shared_ptr<GameObjectManager> GetGameObjectManager();

	void InitMatch(int a_EnemyPlayers, int a_Map, std::shared_ptr<EntityManager> a_EntityManager, std::shared_ptr<ResourceManager> a_ResourceManager);
	void InitEnemyPlayer(std::shared_ptr<EntityManager> a_EntityManager);
	void InitPlayer(std::shared_ptr<EntityManager> a_EntityManager);
	void InitMap( std::shared_ptr<ResourceManager> a_ResourceManager, std::shared_ptr<EntityManager> a_EntityManager);
	void Update(float a_DeltaTime);
	//	void EndMatch();
private:
	std::shared_ptr<PathManager> m_PathManager;
	std::shared_ptr<GameObjectManager> m_GameObjectManager;
	std::vector<std::shared_ptr<EnemyPlayer>> m_PlayersInGame;
	std::vector<std::string> m_Map;
};

inline std::shared_ptr<GameObjectManager> GameMatch::GetGameObjectManager()
{
	return m_GameObjectManager;
}