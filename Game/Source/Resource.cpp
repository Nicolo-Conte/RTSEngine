#include "Resource.h"

 Resource::Resource(glm::vec2 a_position) :
	 m_CurrentAmount(5500)
{
	 m_Transform.Position = a_position;
	 m_Color = ImColor(0, 255, 255);
	 m_Size = 1.3f;
	 m_Name = "Resource Point";
	 m_Transform.Scale = glm::vec2(1.0f, 1.0f);
}

 Resource::Resource(int a_CurrentAmount) :
	 m_CurrentAmount(a_CurrentAmount)
{
	 m_Transform.Position = glm::vec2(24, 32);
}

 Resource::~Resource()
{
}

 void Resource::AddResources(int a_Resources)
 {
	 m_CurrentAmount += a_Resources;
 }

 void Resource::Start()
 {
 }

 void Resource::Update(float a_DeltaTime)
 {
	 a_DeltaTime = 20;
 }
