#include "RujinPCH.h"
#include "GameObject.h"

#include "Component.h"

using namespace rujin;

void GameObject::Start()
{
	for (const auto& component : m_Components)
	{
		component->Start();
	}

	for (const auto& child : m_Children)
	{
		child->Start();
	}
}

void GameObject::LateStart()
{
	for (const auto& component : m_Components)
	{
		component->LateStart();
	}

	for (const auto& child : m_Children)
	{
		child->LateStart();
	}
}

void GameObject::Update()
{
	for (const auto& component : m_Components)
	{
		component->Update();
	}

	for (const auto& child : m_Children)
	{
		child->Update();
	}
}

void GameObject::FixedUpdate()
{
	for (const auto& component : m_Components)
	{
		component->FixedUpdate();
	}

	for (const auto& child : m_Children)
	{
		child->FixedUpdate();
	}
}

void GameObject::OnGui(SDL_Window* pWindow)
{
	for (const auto& component : m_Components)
	{
		component->OnGui(pWindow);
	}

	for (const auto& child : m_Children)
	{
		child->OnGui(pWindow);
	}
}

void GameObject::Draw() const
{
	for (const auto& component : m_Components)
	{
		component->Draw();
	}

	for (const auto& child : m_Children)
	{
		child->Draw();
	}
}

void GameObject::Destroy()
{
	for (const auto& component : m_Components)
	{
		component->Destroy();
	}

	for (const auto& child : m_Children)
	{
		child->Destroy();
	}
}

void GameObject::BroadcastOverlap(const CollisionResult& collision)
{
	for (const auto& component : m_Components)
	{
		component->OnOverlap(collision);
	}
}

Transform& GameObject::GetTransform()
{
	return m_Transform;
}

void GameObject::AddChild(std::unique_ptr<GameObject>& pChild)
{
	assert(pChild); //GameObject invalid.

	pChild->SetParent(this);
	m_Children.push_back(std::move(pChild));
}

/**
 * \brief Detaches from old parent, attaches to new parent.
 */
void GameObject::AttachTo(GameObject* pParent)
{
	//TODO: Handle RootObject attaching to other object (remove from pScene)
	//TODO: Add Possible nullptr to make gameObject root object. (add to pScene)

	assert(pParent); //Parent must be valid.
	assert(m_pParent);

	//1. Ensure i am one of my parent's children
	auto& children = m_pParent->m_Children;
	const auto it = std::ranges::find_if(children, [this](const auto& pChild) { return pChild.get() == this; });

	assert(it != children.end()); //I am not a child of my current parent. You did something terribly wrong if this triggers.

	//2. Move ownership from oldParent, to newParent, become newParent's child.
	pParent->AddChild(*it); // <-- will move ownership of unique_ptr to new parent.

	//3. remove from old parents children.
	//TODO: This might cause a bug (removing during iterating) maybe some kind of queueing system for this
	children.erase(it);

	//4. Update parent ptr
	SetParent(pParent);
}

Scene* GameObject::GetScene() const
{
	if (m_pParent && !m_pScene)
		return m_pParent->GetScene();

	return m_pScene;
}

GameObject* GameObject::GetParent() const
{
	return m_pParent;
}

GameObject* GameObject::GetRootParent()
{
	if (!m_pParent)
		return this;

	return m_pParent->GetRootParent();
}

const std::vector<std::unique_ptr<GameObject>>& GameObject::GetChildren() const 
{
	return m_Children;
}

const GameObject* GameObject::GetChildByName(const std::string& name) const
{
	const auto& it = std::ranges::find_if(m_Children, [&name](const std::unique_ptr<GameObject>& pChild) { return pChild->GetName() == name; });

	if (it != m_Children.end())
		return it->get();

	return nullptr;
}

const std::vector<std::unique_ptr<Component>>& GameObject::GetComponents() const
{
	return m_Components;
}

std::string GameObject::GetName() const
{
	return m_Name;
}

void GameObject::SetParent(GameObject* pParent)
{
	m_pParent = pParent;
}

void GameObject::SetScene(Scene* pScene)
{
	m_pScene = pScene;
}

GameObject::GameObject(const std::string& name)
	: IGameLoopObject()
	, m_Name(name)
{
}
