#include "RujinPCH.h"
#include "GameObject.h"

void rujin::GameObject::Start()
{
	for (const std::shared_ptr<Component> component : m_Components)
	{
		component->Start();
	}
}

void rujin::GameObject::Update()
{
	for (const std::shared_ptr<Component> component : m_Components)
	{
		component->Update();
	}
}

void rujin::GameObject::FixedUpdate()
{
	for (const std::shared_ptr<Component> component : m_Components)
	{
		component->FixedUpdate();
	}
}

void rujin::GameObject::Render() const
{
	for (const std::shared_ptr<Component> component : m_Components)
	{
		component->Render();
	}
}

void rujin::GameObject::SetPosition(const float x, const float y)
{
	m_Transform.SetPosition(x, y, 0.f);
}

void rujin::GameObject::SetParent(const std::weak_ptr<GameObject> parent)
{
	//TODO: maybe do a children check
	m_Parent = parent;
}
