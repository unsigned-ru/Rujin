#include "RujinPCH.h"
#include "GameObject.h"

using namespace rujin;

void GameObject::Start()
{
	for (const auto& component : m_Components)
	{
		component->Start();
	}
}

void GameObject::Update()
{
	for (const auto& component : m_Components)
	{
		component->Update();
	}
}

void GameObject::FixedUpdate()
{
	for (const auto& component : m_Components)
	{
		component->FixedUpdate();
	}
}

void GameObject::OnGui(SDL_Window* pWindow)
{
	for (const auto& component : m_Components)
	{
		component->OnGui(pWindow);
	}
}

void GameObject::Render() const
{
	for (const auto& component : m_Components)
	{
		component->Render();
	}
}

void GameObject::Destroy()
{
	for (const auto& component : m_Components)
	{
		component->Destroy();
	}
}

TransformComponent* GameObject::GetTransform() const
{
	return m_pTransformComp;
}

void GameObject::SetParent(GameObject* pParent)
{
	//TODO: attach all children to this new parent, a new game object now owns all sub game objects
	m_pParent = pParent;
}

GameObject* GameObject::GetParent() const
{
	return m_pParent;
}

std::string GameObject::GetName() const
{
	return m_Name;
}

GameObject::GameObject(const std::string& name)
	: IGameLoopObject()
	, m_Name(name)
	, m_pTransformComp(AddComponent<TransformComponent>())
{
}

