#include "RujinPCH.h"
#include "GameObject.h"

#include "Component.h"
#include "Scene.h"

using namespace rujin;

void GameObject::Start()
{
	m_Components.Iterate
	(
		[](const std::unique_ptr<Component>& pComponent)
		{
			pComponent->Start();
		}
	);

	m_Children.Iterate
	(
		[](const std::unique_ptr<GameObject>& pChild)
		{
			pChild->Start();
		}
	);
}

void GameObject::LateStart()
{
	m_Components.Iterate
	(
		[](const std::unique_ptr<Component>& pComponent)
		{
			pComponent->LateStart();
		}
	);

	m_Children.Iterate
	(
		[](const std::unique_ptr<GameObject>& pChild)
		{
			pChild->LateStart();
		}
	);

	m_IsInitialized = true;
}

void GameObject::Update()
{
	m_EnabledComponents.Iterate
	(
		[](Component* pComponent)
		{
			pComponent->Update();
		}
	);

	m_EnabledChildren.Iterate
	(
		[](GameObject* pGameObject)
		{
			pGameObject->Update();
		}
	);
}

void GameObject::FixedUpdate()
{
	m_Components.SetIterating(true);
	m_EnabledComponents.Iterate
	(
		[](Component* pComponent)
		{
			pComponent->FixedUpdate();
		}
	);
	m_Components.SetIterating(false);

	m_Children.SetIterating(true);
	m_EnabledChildren.Iterate
	(
		[](GameObject* pGameObject)
		{
			pGameObject->FixedUpdate();
		}
	);
	m_Children.SetIterating(false);
}

void GameObject::OnGui(SDL_Window* pWindow)
{
	m_EnabledComponents.Iterate
	(
		[pWindow](Component* pComponent)
		{
			pComponent->OnGui(pWindow);
		}
	);

	m_EnabledChildren.Iterate
	(
		[pWindow](GameObject* pGameObject)
		{
			pGameObject->OnGui(pWindow);
		}
	);
}

void GameObject::Draw() const
{
	m_EnabledComponents.Iterate
	(
		[](const Component* pComponent)
		{
			pComponent->Draw();
		}
	);

	m_EnabledChildren.Iterate
	(
		[](const GameObject* pGameObject)
		{
			pGameObject->Draw();
		}
	);
}

void GameObject::Destroy()
{
	m_PendingDestroy = true;

	//let all our components know, we are going to be destroyed...
	m_Components.Iterate
	(
		[](const std::unique_ptr<Component>& pComponent)
		{
			pComponent->m_PendingDestroy = true;
		}
	);

	//let all our children know they are going to be destroyed....
	m_Children.Iterate
	(
		[](const std::unique_ptr<GameObject>& pChild)
		{
			pChild->m_PendingDestroy = true;

			//let all components of our children know they are going to be destroyed...
			pChild->m_Components.Iterate
			(
				[](const std::unique_ptr<Component>& pComponent)
				{
					pComponent->m_PendingDestroy = true;
				}
			);
		}
	);

	if (m_pParent)
		m_pParent->RemoveChild(this);
	else
		m_pScene->RemoveGameObject(this);
}

void GameObject::OnEnable()
{
	if (m_PendingDestroy) 
		return;

	StatefullObject::OnEnable();

	if (m_pParent)
	{
		auto& parentEnabledChildren = m_pParent->m_EnabledChildren.GetVector();
		if (std::find(parentEnabledChildren.begin(), parentEnabledChildren.end(), this) == parentEnabledChildren.end())
		{
			//we are not in the enabled children, disable
			parentEnabledChildren.emplace_back(this);
			m_pParent->m_DisabledChildren.Remove(this);
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

			m_pScene->GetDisabledGameObjects().Remove(this);
		}
	}
}

void GameObject::OnDisable()
{
	if (m_PendingDestroy) 
		return;

	StatefullObject::OnDisable();

	if (m_pParent)
	{
		auto& parentDisabledChildren = m_pParent->m_DisabledChildren.GetVector();
		if (std::find(parentDisabledChildren.begin(), parentDisabledChildren.end(), this) == parentDisabledChildren.end())
		{
			//we are not in the disabled children, enable...
			parentDisabledChildren.emplace_back(this);
			m_pParent->m_EnabledChildren.Remove(this);
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

			m_pScene->GetEnabledGameObjects().Remove(this);
		}
	}
}

void GameObject::BroadcastOverlap(const CollisionResult& collision)
{
	if (m_PendingDestroy) 
		return;

	m_EnabledComponents.Iterate
	(
		[&collision](Component* pComponent)
		{
			pComponent->OnOverlap(collision);
		}
	);
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

	m_Children.GetVector().emplace_back(std::unique_ptr<GameObject>(pChild));
	m_EnabledChildren.GetVector().emplace_back(pChild);
}

void GameObject::RemoveChild(GameObject* pChild)
{
	ASSERT(pChild);

	if (pChild->IsEnabled())
		m_EnabledChildren.Remove(pChild);
	else
		m_DisabledChildren.Remove(pChild);

	m_Children.RemoveIf([pChild](const std::unique_ptr<GameObject>& child) { return child.get() == pChild; });
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

const DeferredRemovalVector<std::unique_ptr<GameObject>>& GameObject::GetChildren()
{
	return m_Children;
}

DeferredRemovalVector<GameObject*>& GameObject::GetEnabledChildren()
{
	return m_EnabledChildren;
}

DeferredRemovalVector<GameObject*>& GameObject::GetDisabledChildren()
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

const DeferredRemovalVector<std::unique_ptr<Component>>& GameObject::GetComponents()
{
	return m_Components;
}

DeferredRemovalVector<Component*>& GameObject::GetEnabledComponents()
{
	return m_EnabledComponents;
}

DeferredRemovalVector<Component*>& GameObject::GetDisabledComponents()
{
	return m_DisabledComponents;
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

GameObject::~GameObject() = default;
