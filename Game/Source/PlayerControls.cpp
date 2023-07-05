#include "PlayerControls.h"
#include "EntityManager/Camera.h"
PlayerControls::PlayerControls(std::shared_ptr<GameObjectManager> a_GameObjectManager, std::shared_ptr<EntityManager> a_EntityManager, std::shared_ptr<SFMLRenderer> a_SFMLRenderer)
{
	m_SelectedObject = nullptr;
	m_GameObjectManager = a_GameObjectManager;
	m_EntityManager = a_EntityManager;
	m_SFMLRenderer = a_SFMLRenderer;
	PlaceHouse   = false;
	PlaceBarrack = false;
}

PlayerControls::~PlayerControls()
{
}

int counter = 0;
void PlayerControls::Update(float a_DeltaTime)
{
	a_DeltaTime;

	ImGui::Begin("Player values");
	ImGui::Text("Resources: %i", m_GameObjectManager->GetPlayer()->GetFaction()->GetCurrency());
	ImGui::Text("Current Population: %i", m_GameObjectManager->GetPlayer()->GetCurrentPopulation());
	ImGui::Text("MaxPopulation: %i", m_GameObjectManager->GetPlayer()->GetMaxPopulation());
	ImGui::End();
	/*
		Update camera?
	*/
	if (sf::Mouse::getPosition(m_SFMLRenderer->GetWindow()).x >= 1260) {
		m_EntityManager->GetCamera()->SetDirection(glm::vec2(250.0f ,0));
		
	}
	if (sf::Mouse::getPosition(m_SFMLRenderer->GetWindow()).x <= 20) {
		m_EntityManager->GetCamera()->SetDirection(glm::vec2(-150.0f, 0));
		
	}
	if (sf::Mouse::getPosition(m_SFMLRenderer->GetWindow()).y>= 780) {
		m_EntityManager->GetCamera()->SetDirection(glm::vec2(0, 250.0f));
		
	}
	if (sf::Mouse::getPosition(m_SFMLRenderer->GetWindow()).y <= 20) {
		m_EntityManager->GetCamera()->SetDirection(glm::vec2(0, -150.0f));
		
	}


	for (int i = 0; i < m_EntityManager->GetObjectUpdate().size(); i++) {

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
		
			sf::Vector2f mouse = m_SFMLRenderer->GetWindow().mapPixelToCoords(sf::Mouse::getPosition(m_SFMLRenderer->GetWindow()));
		
			sf::FloatRect bounds(sf::Vector2f(m_EntityManager->GetObjectUpdate()[i]->GetTransform().Position.x,m_EntityManager->GetObjectUpdate()[i]->GetTransform().Position.y), sf::Vector2f(64, 64));
			Object* a_Object = m_EntityManager->GetObjectUpdate()[i].get();
			
			if (bounds.contains(mouse) && dynamic_cast<GameObject*>(a_Object)->GetPlayer() == m_GameObjectManager->GetPlayer())
			{
				m_SelectedObject = a_Object;
				if  (a_Object->GetName() == "Melee Unit")  
				{
					m_GameObjectManager->SetCombatUnit((MeleeUnit*)a_Object);
				}
				else if (a_Object->GetName() == "Ranged Unit")
				{
					m_GameObjectManager->SetCombatUnit((RangedUnit*)a_Object);
				}
				else if (a_Object->GetName() == "Builder Unit")
				{
					
					 m_GameObjectManager->SetBuilderUnit((BuilderUnit*)a_Object);

				}
				else if (a_Object->GetName() == "Barrack")
				{
					m_GameObjectManager->SetBarrack((Barrack*)(a_Object));

				}
				else if (a_Object->GetName() == "House")
				{
					m_GameObjectManager->SetHouse((House*)(a_Object));
					
				}
				else
				{
					//return m_ResourceIndex;
					m_GameObjectManager->SetNoSelection();
					m_SelectedObject = nullptr;
				}
				printf("Hit!\n");
				break;
			}
		}
	
		if ((m_SelectedObject != nullptr) && (sf::Mouse::isButtonPressed(sf::Mouse::Right) == true))
		{

			sf::Vector2f mouse = m_SFMLRenderer->GetWindow().mapPixelToCoords(sf::Mouse::getPosition(m_SFMLRenderer->GetWindow()));

			sf::FloatRect bounds(sf::Vector2f(m_EntityManager->GetObjectUpdate()[i]->GetTransform().Position.x, m_EntityManager->GetObjectUpdate()[i]->GetTransform().Position.y), sf::Vector2f(64, 64));
			Object* a_Object = m_EntityManager->GetObjectUpdate()[i].get();
			if (bounds.contains(mouse))
			{
				if (m_SelectedObject->GetName() == "Melee Unit" || m_SelectedObject->GetName() == "Ranged Unit")
				{
					if (a_Object->GetName() == "Melee Unit" || a_Object->GetName() == "Ranged Unit" || a_Object->GetName() == "Builder Unit" || a_Object->GetName() == "Barrack" || a_Object->GetName() == "House")
					{
						m_GameObjectManager->ExecuteOrders((GameObject*)a_Object);
						//m_GameObjectManager->SetCombatUnit((MeleeUnit*)a_Object);
						return;
					}
				}
				if (m_SelectedObject->GetName() == "Builder Unit")
				{
					if (a_Object->GetName() == "Resource Point")
					{
						m_GameObjectManager->ExecuteOrders((GameObject*)a_Object);
						return;
					}
				}
			}
			else if (m_SelectedObject->GetName() == "Melee Unit" || m_SelectedObject->GetName() == "Ranged Unit" || m_SelectedObject->GetName() == "Builder Unit")
			{
				m_GameObjectManager->ExecuteOrders(glm::vec2(mouse.x, mouse.y));
			}
			if (m_SelectedObject->GetName() == "House")
			{

			}
			
		
		/*
		* if selected a combat unit and click on enemy unit or building execute attack
		  if selected a unit and click on a empty space execute move
		*/
	/*
		Pass to  GameObjectManager the order to execute with right button of the mouse
	*/
		
		}

	}
	
		ObjectWindow(m_SelectedObject);
	
	ImGui::Render();
}


void PlayerControls::ObjectWindow(Object* a_SelectedObject)
{
	ImGui::Begin("Player Controls");
	if (PlaceHouse == true)
	{
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			m_GameObjectManager->PlaceBuilding(glm::vec2(m_SFMLRenderer->GetWindow().mapPixelToCoords(sf::Mouse::getPosition(m_SFMLRenderer->GetWindow())).x,
				m_SFMLRenderer->GetWindow().mapPixelToCoords(sf::Mouse::getPosition(m_SFMLRenderer->GetWindow())).y), 1, m_GameObjectManager->GetPlayer());
			PlaceHouse = false;
		}
	}

	if (PlaceBarrack == true)
	{
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			m_GameObjectManager->PlaceBuilding(glm::vec2(m_SFMLRenderer->GetWindow().mapPixelToCoords(sf::Mouse::getPosition(m_SFMLRenderer->GetWindow())).x,
				m_SFMLRenderer->GetWindow().mapPixelToCoords(sf::Mouse::getPosition(m_SFMLRenderer->GetWindow())).y), 2, m_GameObjectManager->GetPlayer());
			PlaceBarrack = false;
		}
	}

	if (a_SelectedObject != nullptr)
	{

		
		if (a_SelectedObject->GetName() == "Builder Unit")
		{
			
			//sf::Vector2f mouse = m_SFMLRenderer->GetWindow().mapPixelToCoords(sf::Mouse::getPosition(m_SFMLRenderer->GetWindow()));

			if (ImGui::Button("Build House, cost 100", ImVec2(0, 50)))
			{
				
					PlaceHouse = true;
					
				
			}
			if (ImGui::Button("Build Barrack, cost 200", ImVec2(0, 100)))
			{
				
					PlaceBarrack = true;
				
			}
			ImGui::Text("Resources collected: %i", dynamic_cast<BuilderUnit*>(a_SelectedObject)->GetResourcesCollected());
			
		}
		else if (a_SelectedObject->GetName() == "House")
		{
			
			if (ImGui::Button("Produce Builder Unit, cost 50", ImVec2(0, 200)))
			{
				m_GameObjectManager->ExecuteOrders(1);
			}
			ImGui::Text("Time for next units: %f", dynamic_cast<House*>(a_SelectedObject)->GetCurrentUnitTime());
		}
		else if (a_SelectedObject->GetName() == "Barrack")
		{
			
			if (ImGui::Button("Produce Melee Unit, cost 75", ImVec2(0, 50)))
			{
				m_GameObjectManager->ExecuteOrders(1);
			}
			if (ImGui::Button("Produce Ranged Unit, cost 100", ImVec2(0, 100)))
			{
				m_GameObjectManager->ExecuteOrders(2);
			}
			ImGui::Text("Time for next units: %f", dynamic_cast<Barrack*>(a_SelectedObject)->GetCurrentUnitTime());
		}
		ImGui::End();
	}
	else
	{
		
		//ImGui::Begin("Player Controls");
		ImGui::End();
		return;
	}
}