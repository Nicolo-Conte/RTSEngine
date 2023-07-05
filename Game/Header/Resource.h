#pragma once
#include "GameObject.h"
/// <summary>
/// child class of Object class; 
/// represents the collection point of resources.
/// </summary>
class Resource : public GameObject
{
private:
	int m_CurrentAmount; //indicates the remaining number of resources to be collected

public:

	//Constructors & Destructor
	Resource(){}
	Resource(glm::vec2 a_position);
	Resource(int a_CurrentAmount);
	~Resource();

	//Setter & Getter
	int GetCurrentAmount();
	void SetCurrentAmount(int a_CurrentAmount);

	//other functions
	void AddResources(int a_Resources);
	void Start();
	void Update(float a_DeltaTime);
};

inline int Resource::GetCurrentAmount()
{
	return m_CurrentAmount;
}


inline void Resource::SetCurrentAmount(int a_CurrentAmount)
{
	 m_CurrentAmount = a_CurrentAmount;
}




