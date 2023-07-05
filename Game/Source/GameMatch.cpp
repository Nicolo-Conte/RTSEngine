#include "GameMatch.h"


GameMatch::GameMatch()
{
	m_Map.push_back("MapTest50x50.json");
	m_Map.push_back("SmallTestMap.json");
	m_Map.push_back("bigmapdata.json");
	srand((unsigned int)time(NULL));
}

GameMatch::~GameMatch()
{
}

void GameMatch::InitMatch(int a_EnemyPlayers, int a_Map, std::shared_ptr<EntityManager> a_EntityManager, std::shared_ptr<ResourceManager> a_ResourceManager)
{
	m_PathManager = std::make_shared<PathManager>();
	 std::string smallTestMap(m_Map[a_Map]);
	a_ResourceManager->LoadWorldMap(smallTestMap);
	m_GameObjectManager = std::make_shared<GameObjectManager>(a_EntityManager, m_PathManager);
	InitMap(a_ResourceManager, a_EntityManager);
	InitPlayer(a_EntityManager);
	for (int i = 0; i < a_EnemyPlayers; i++)
	{
		InitEnemyPlayer(a_EntityManager);
	}
	
}

void GameMatch::InitEnemyPlayer(std::shared_ptr<EntityManager> a_EntityManager)
{
	//srand((unsigned int)time(NULL));
	int randx = rand() % 95 + 3;
	int randy = rand() % 95 + 3;
	std::shared_ptr<Faction> fac = std::make_shared<Faction>();
	std::shared_ptr<EnemyPlayer> Eplayer = std::make_shared<EnemyPlayer>(fac);
	std::shared_ptr<House> house1 = std::make_shared<House>(glm::vec2(randx * 16, randy * 16), m_GameObjectManager, Eplayer);
	std::shared_ptr<BuilderUnit> builder1 = std::make_shared<BuilderUnit>(m_PathManager, m_GameObjectManager, glm::vec2((randx -2) * 16, (randy + 2) * 16), Eplayer);
	std::shared_ptr<BuilderUnit> builder2 = std::make_shared<BuilderUnit>(m_PathManager, m_GameObjectManager, glm::vec2((randx+2 ) * 16, (randy + 2) * 16), Eplayer);
	fac->AddUnit(builder1);
	fac->AddUnit(builder2);
	fac->AddBuilding(house1);
	Eplayer->AddBuilderUnit(builder1);
	Eplayer->AddBuilderUnit(builder2);
	Eplayer->AddHouse(house1);
	house1->Build();
	m_GameObjectManager->AddPlayer(Eplayer);
	m_GameObjectManager->AddEnemyPlayer(Eplayer);
	a_EntityManager->AddFaction(fac);
	a_EntityManager->AddObject(builder1);
	a_EntityManager->AddObject(builder2);
	a_EntityManager->AddObject(house1);
	m_PlayersInGame.push_back(Eplayer);
	Eplayer->SetResourcePoint(m_GameObjectManager->SearchResourcePosition(Eplayer));
	return;
}

void GameMatch::InitPlayer(std::shared_ptr<EntityManager> a_EntityManager)
{

	int randx = rand() % 95 + 3;
	int randy = rand() % 95 + 3;
	std::shared_ptr<Faction> fac = std::make_shared<Faction>();
	std::shared_ptr<Player> player = std::make_shared<Player>(fac);
	std::shared_ptr<House> house1 = std::make_shared<House>(glm::vec2(randx * 16, randy * 16), m_GameObjectManager, player);
	std::shared_ptr<BuilderUnit> builder1 = std::make_shared<BuilderUnit>(m_PathManager, m_GameObjectManager, glm::vec2((randx - 2) * 16, (randy + 2) * 16), player);
	std::shared_ptr<BuilderUnit> builder2 = std::make_shared<BuilderUnit>(m_PathManager, m_GameObjectManager, glm::vec2((randx + 2) * 16, (randy + 2) * 16), player);
	fac->AddUnit(builder1);
	fac->AddUnit(builder2);
	fac->AddBuilding(house1);
	player->AddBuilderUnit(builder1);
	player->AddBuilderUnit(builder2);
	player->AddHouse(house1);
	m_GameObjectManager->AddPlayer(player);
	house1->Build();
	a_EntityManager->AddFaction(fac);
	a_EntityManager->AddObject(builder1);
	a_EntityManager->AddObject(builder2);
	a_EntityManager->AddObject(house1);

	a_EntityManager->GetCamera()->SetPosition(house1->GetTransform().Position);

	return;
}

void GameMatch::InitMap(std::shared_ptr<ResourceManager> a_ResourceManager, std::shared_ptr<EntityManager> a_EntityManager)
{
	m_PathManager->InitPathManger(a_ResourceManager);
	m_PathManager->BuildNodeGraph();
	for (int x = 0; x < a_ResourceManager->GetTileMaps()[0]->GetMapWidth(); x++)
		for (int y = 0; y < a_ResourceManager->GetTileMaps()[0]->GetMapHeight(); y++)
		{
			int f = a_ResourceManager->GetTileMaps()[0]->GetCollisionMap()[(x* a_ResourceManager->GetTileMaps()[0]->GetMapWidth()) + y];
			const int RESOURCEVALUE = 2;
			const int RESOURCESIZE = 64;
			if (f == RESOURCEVALUE)		//63 == Resource Point
			{
				std::shared_ptr<Resource> r1 = std::make_shared<Resource>(glm::vec2((y*RESOURCESIZE), (x*RESOURCESIZE)));
				m_GameObjectManager->AddResource(r1);
				a_EntityManager->AddObject(r1);
			}
		}
}

void GameMatch::Update(float a_DeltaTime)
{
	for (size_t i = 0; i < m_PlayersInGame.size(); i++)
		m_PlayersInGame[i]->Update(a_DeltaTime);
}
