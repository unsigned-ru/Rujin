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
	for (const auto& component : m_Components)
	{
		component->Update();
	}
}

void rujin::GameObject::FixedUpdate()
{
	for (const auto& component : m_Components)
	{
		component->FixedUpdate();
	}
}

void rujin::GameObject::Render() const
{
	for (const auto& component : m_Components)
	{
		component->Render();
	}
}

void rujin::GameObject::Destroy()
{
	for (const auto& component : m_Components)
	{
		component->Destroy();
	}
}

std::weak_ptr<rujin::TransformComponent> rujin::GameObject::GetTransform()
{
	return GetComponent<TransformComponent>();
}

void rujin::GameObject::SetParent(const std::weak_ptr<GameObject> parent)
{
	//TODO: maybe do a children check
	m_Parent = parent;
}

std::weak_ptr<rujin::GameObject> rujin::GameObject::GetParent() const
{
	return m_Parent;
}

std::string rujin::GameObject::GetName() const
{
	return m_Name;
}

rujin::GameObject::GameObject(const std::string& name)
	: MonoBehaviour()
	, m_Name(name)
{
}

