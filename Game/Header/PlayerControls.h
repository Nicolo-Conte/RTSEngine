#include "GameObjectManager.h"
#include "SFMLRenderer.h"
#include "EntityManager.h"

#pragma once
class PlayerControls
{

public:
	PlayerControls(std::shared_ptr<GameObjectManager> a_GameObjectManager, std::shared_ptr<EntityManager> a_EntityManager, std::shared_ptr<SFMLRenderer> a_SFMLRenderer);
	~PlayerControls();


	Object* GetSelectedObject();
	void SetSelectedObject(Object* a_SelectedObject);

	void ObjectWindow(Object* a_SelectedObject);

	void Update(float a_DeltaTime);
private:

	Object* m_SelectedObject;
	std::shared_ptr<GameObjectManager> m_GameObjectManager;
	std::shared_ptr<EntityManager> m_EntityManager;
	std::shared_ptr<SFMLRenderer> m_SFMLRenderer;
	bool PlaceHouse;
	bool PlaceBarrack;
};

inline Object* PlayerControls::GetSelectedObject()
{
	return m_SelectedObject;
}

inline void PlayerControls::SetSelectedObject(Object* a_SelectedObject)
{
	m_SelectedObject = a_SelectedObject;
}
