#pragma once
#include <memory>
#include "ObjectValues.h"
#include "EntityManager/Object.h"
#include <string>

class GameObjectManager;
class Player;


/// <summary>
/// child class of Object class;
/// Base class of the specific entities in game ( Buildings, Units)
/// </summary>
class GameObject : public Object 
{
protected:

	int m_CurrentHealth;						//Integer of current amount of Health that a Unit or a Building have
	float m_CurrentConstructionTime;			//number of how many Time remaining for the construction of the Building or Unit. For Buildings this attribute can decrease only if 
												//there are at least one BuilderUnit who are constructing
	bool m_UnderAttack;							/*Boolean values which indicate if a Game_Object is under Attack 
												 True if an enemy unit is dealing damage to the Game_Object, else if false */
	std::shared_ptr<GameObjectManager> m_GameObjectManager;
	std::shared_ptr<Player> m_Player;
public:

	//Constructors & Destructor
	GameObject();
	GameObject(std::shared_ptr<GameObjectManager> a_GameObjectManager, std::shared_ptr<Player> a_Player);
	GameObject(int a_Health, float a_Time, bool a_Attack, std::shared_ptr<Player> a_Player);
	~GameObject();

	//Setter & Getter
	int GetCurrentHealth();
	float GetCurrentConstructionTime(); 
	bool GetUnderAttack();
	std::shared_ptr<GameObjectManager> GetGameObjectManager();
	void SetCurrentHealth(int a_Health);
	void SetCurrentConstructionTime(float a_Time);
	void SetUnderAttack(bool a_Attack);
	std::shared_ptr<Player> GetPlayer();
	void SetPlayer(std::shared_ptr<Player> a_Player);

	//other functions
	void Start();
};


inline int GameObject::GetCurrentHealth()
{
	return m_CurrentHealth;
}

inline float GameObject::GetCurrentConstructionTime()
{
	return m_CurrentConstructionTime;
}

inline bool GameObject::GetUnderAttack()
{
	return m_UnderAttack;
}



inline void GameObject::SetCurrentHealth(int a_Health)
{
	m_CurrentHealth = a_Health;
}

inline void GameObject::SetCurrentConstructionTime(float a_Time)
{
	m_CurrentConstructionTime = a_Time;
}

inline void GameObject::SetUnderAttack(bool a_Attack)
{
	m_UnderAttack = a_Attack;

}

inline std::shared_ptr<Player> GameObject::GetPlayer()
{
	return m_Player;
}

inline void GameObject::SetPlayer(std::shared_ptr<Player> a_Player)
{
	m_Player = a_Player;
}

