#include "RujinPCH.h"
#include "GameObject.h"

#include "Component.h"
#include "Scene.h"

using namespace rujin;

void GameObject::Start()
{
	for (const std::unique_ptr<Component>& pComponent : m_Components.GetVector())
	{
		pComponent->Start();
	}

	for (const std::unique_ptr<GameObject>& pChild : m_Children.GetVector())
	{
		pChild->Start();
	}
}

void GameObject::LateStart()
{
	for (const std::unique_ptr<Component>& pComponent : m_Components.GetVector())
	{
		pComponent->LateStart();
	}

	for (const std::unique_ptr<GameObject>& pChild : m_Children.GetVector())
	{
		pChild->LateStart();
	}

	m_IsInitialized = true;
}

void GameObject::Update()
{
	for (Component* pComponent : m_EnabledComponents.GetVector())
	{
		pComponent->Update();
	}

	for (GameObject* pChild : m_EnabledChildren.GetVector())
	{
		pChild->Update();
	}
}

void GameObject::FixedUpdate()
{
	for (Component* pComponent : m_EnabledComponents.GetVector())
	{
		pComponent->FixedUpdate();
	}

	for (GameObject* pChild : m_EnabledChildren.GetVector())
	{
		pChild->FixedUpdate();
	}
}

void GameObject::ProcessAdditionsAndRemovals()
{
	m_DisabledComponents.ProcessRemovals();
	m_EnabledComponents.ProcessRemovals();
	m_Components.ProcessRemovals();

	m_DisabledChildren.ProcessRemovals();
	m_EnabledChildren.ProcessRemovals();
	m_Children.ProcessRemovals();

	for (GameObject* pChild : m_EnabledChildren.GetVector())
	{
		pChild->ProcessAdditionsAndRemovals();
	}

	for (GameObject* pChild : m_EnabledChildren.GetElementsToAdd())
	{
		pChild->ProcessAdditionsAndRemovals();
	}

	m_DisabledComponents.ProcessAdditions();
	m_EnabledComponents.ProcessAdditions();
	m_Components.ProcessAdditions();

	m_DisabledChildren.ProcessAdditions();
	m_EnabledChildren.ProcessAdditions();
	m_Children.ProcessAdditions();
}

void GameObject::OnGui(SDL_Window* pWindow)
{
	for (Component* pComponent : m_EnabledComponents.GetVector())
	{
		pComponent->OnGui(pWindow);
	}

	for (GameObject* pChild : m_EnabledChildren.GetVector())
	{
		pChild->OnGui(pWindow);
	}
}

void GameObject::Draw() const
{
	for (const Component* pComponent : m_EnabledComponents.GetVector())
	{
		pComponent->Draw();
	}

	for (const GameObject* pChild : m_EnabledChildren.GetVector())
	{
		pChild->Draw();
	}
}

void GameObject::Destroy()
{
	LOG_DEBUG_("Destroying GO: {}", m_Name);

	if (m_PendingDestroy)
		return;

	m_PendingDestroy = true;


	if (m_pParent)
		m_pParent->RemoveChild(this);
	else
		m_pScene->RemoveGameObject(this);
}

void GameObject::OnEnable()
{
	StatefullObject::OnEnable();

	if (m_pParent)
	{
		auto& parentEnabledChildren = m_pParent->m_EnabledChildren.GetVector();
		if (std::find(parentEnabledChildren.begin(), parentEnabledChildren.end(), this) == parentEnabledChildren.end())
		{
			//we are not in the enabled children, disable
			parentEnabledChildren.emplace_back(this);
			m_pParent->m_DisabledChildren.PendRemove(this);
		}
	}
	else if (m_pScene) //we don't have a parent, do we have a scene?
	{
		//we are a root gameobject.
		auto& sceneEnabledGameobjects = m_pScene->GetEnabledGameObjects().GetVector();
		if (std::find(sceneEnabledGameobjects.begin(), sceneEnabledGameobjects.end(), this) == sceneEnabledGameobjects.end())
		{
			//we are disabled, enable
			sceneEnabledGameobjects.emplace_back(this);

			m_pScene->GetDisabledGameObjects().PendRemove(this);
		}
	}
}

void GameObject::OnDisable()
{
	StatefullObject::OnDisable();

	if (m_pParent)
	{
		auto& parentDisabledChildren = m_pParent->m_DisabledChildren.GetVector();
		if (std::find(parentDisabledChildren.begin(), parentDisabledChildren.end(), this) == parentDisabledChildren.end())
		{
			//we are not in the disabled children, enable...
			parentDisabledChildren.emplace_back(this);
			m_pParent->m_EnabledChildren.PendRemove(this);
		}
	}
	else if (m_pScene) //we don't have a parent, do we have a scene?
	{
		//we are a root gameobject.
		auto& sceneDisabledGameObjects = m_pScene->GetDisabledGameObjects().GetVector();
		if (std::find(sceneDisabledGameObjects.begin(), sceneDisabledGameObjects.end(), this) == sceneDisabledGameObjects.end())
		{
			//we are enabled, disable
			sceneDisabledGameObjects.emplace_back(this);

			m_pScene->GetEnabledGameObjects().PendRemove(this);
		}
	}
}

void GameObject::BroadcastOverlap(const CollisionResult& collision)
{
	for (Component* pComponent : m_EnabledComponents.GetVector())
	{
		pComponent->OnOverlap(collision);
	}
}

Transform& GameObject::GetTransform()
{
	return m_Transform;
}

void GameObject::AddChild(GameObject* pChild)
{
	ASSERT(pChild);

	pChild->SetParent(this);

	if (m_IsInitialized)
	{
		//this gameobject is added as child post initialization.
		pChild->GetTransform().UpdateSelfAndChildren();
		pChild->Start();
	}

	m_Children.PendPushBack(std::unique_ptr<GameObject>(pChild));
	m_EnabledChildren.PendPushBack(pChild);
}

void GameObject::RemoveChild(GameObject* pChild)
{
	ASSERT(pChild);

	if (pChild->IsEnabled())
		m_EnabledChildren.PendRemove(pChild);
	else
		m_DisabledChildren.PendRemove(pChild);

	m_Children.PendRemove(std::ranges::find_if(m_Children.GetVector(), [pChild](const std::unique_ptr<GameObject>& child) { return child.get() == pChild; })->get());
}

///**
// * \brief Detaches from old parent, attaches to new parent.
// */
//void GameObject::AttachTo(GameObject* pParent)
//{
//	//TODO: Handle RootObject attaching to other object (remove from pScene)
//	//TODO: Add Possible nullptr to make gameObject root object. (add to pScene)
//
//	ASSERT(pParent);
//	ASSERT(m_pParent);
//
//	//1. Ensure i am one of my parent's children
//	auto& children = m_pParent->m_Children;
//	const auto it = std::ranges::find_if(children, [this](const auto& pChild) { return pChild.get() == this; });
//
//	ASSERT(it != children.end()); //I am not a child of my current parent. You did something terribly wrong if this triggers.
//
//	//2. Move ownership from oldParent, to newParent, become newParent's child.
//	pParent->AddChild(it->get()); // <-- will move ownership of unique_ptr to new parent.
//
//	//3. remove from old parents children.
//	//TODO: This might cause a bug (removing during iterating) maybe some kind of queueing system for this
//	//unique_ptr bug here.
//	RemoveChild(it->get());
//
//	//4. Update parent ptr
//	SetParent(pParent);
//}

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

const DeferredVector<std::unique_ptr<GameObject>, GameObject*>& GameObject::GetChildren()
{
	return m_Children;
}

DeferredVector<GameObject*, GameObject*>& GameObject::GetEnabledChildren()
{
	return m_EnabledChildren;
}

DeferredVector<GameObject*, GameObject*>& GameObject::GetDisabledChildren()
{
	return m_DisabledChildren;
}

const GameObject* GameObject::GetChildByName(const std::string& name) const
{
	const auto& children = m_Children.GetVector();
	const auto& it = std::ranges::find_if(children, [&name](const std::unique_ptr<GameObject>& pChild) { return pChild->GetName() == name; });

	if (it != children.end())
		return it->get();

	return nullptr;
}

const DeferredVector<std::unique_ptr<Component>, Component*>& GameObject::GetComponents()
{
	return m_Components;
}

DeferredVector<Component*, Component*>& GameObject::GetEnabledComponents()
{
	return m_EnabledComponents;
}

DeferredVector<Component*, Component*>& GameObject::GetDisabledComponents()
{
	return m_DisabledComponents;
}

std::string GameObject::GetName() const
{
	return m_Name;
}

bool GameObject::HasTag(const std::string& tag) const
{
	return m_Tags.contains(tag);
}

void GameObject::AddTag(const std::string& tag)
{
	m_Tags.emplace(tag);
}

void GameObject::RemoveTag(const std::string& tag)
{
	m_Tags.erase(tag);
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

GameObject::~GameObject()
{
	LOG_DEBUG_("GameObject Destroyed: {}", m_Name);
}
 