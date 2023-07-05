#include "GameObject.h"
#include "GameObjectManager.h"
GameObject::GameObject() :
	m_CurrentHealth(0),
	m_CurrentConstructionTime(0.0f),
	m_UnderAttack(false),
	m_GameObjectManager(nullptr),
	m_Player(nullptr)
{}

GameObject::GameObject(std::shared_ptr<GameObjectManager> a_GameObjectManager, std::shared_ptr<Player> a_Player) :
	m_CurrentHealth(0),
	m_CurrentConstructionTime(0.0f),
	m_UnderAttack(false),
	m_GameObjectManager(a_GameObjectManager),
	m_Player(a_Player)
{}

GameObject::GameObject(int a_Health, float a_Time, bool a_Attack, std::shared_ptr<Player> a_Player) :
	m_CurrentHealth(a_Health),
	m_CurrentConstructionTime(a_Time),
	m_UnderAttack(a_Attack),
	m_GameObjectManager(nullptr),
	m_Player(a_Player)
{}

GameObject::~GameObject()
{
}

inline void GameObject::Start()
{
}

std::shared_ptr<GameObjectManager> GameObject::GetGameObjectManager()
{
	return m_GameObjectManager;
}