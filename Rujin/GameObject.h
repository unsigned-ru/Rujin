#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <set>

#include "IGameLoopObject.h"
#include "Transform.h"
#include "Component.h"
#include "DefferedRemovalVector.h"
#include "StatefullObject.h"

namespace rujin
{
	class Scene;
	/**
	 * \brief GameObject class given functionality by attaching components.
	 * \note
	 * - OWNS children
	 * - OWNS components
	 * - DOES NOT OWN pointer to parent
	 * - Parent nullptr if at root of scene.
	 */
	class GameObject final : public IGameLoopObject, public StatefullObject
	{
	public:
		explicit GameObject(const std::string& name);
		~GameObject() override;
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		void Start() override;
		void LateStart() override;
		void Update() override;
		void FixedUpdate() override;
		void ProcessAdditionsAndRemovals();
		void OnGui(SDL_Window* pWindow) override;
		void Draw() const override;

		void Destroy() override;

		void OnEnable() override;
		void OnDisable() override;

		void BroadcastOverlap(const CollisionResult& collision);

		template<typename ComponentType, typename = std::enable_if<std::is_base_of_v<Component, ComponentType>>>
		ComponentType* AddComponent(ComponentType* pComponent)
		{
			if (!pComponent)
				return nullptr;

			pComponent->m_pGameObject = this;
			m_Components.PendPushBack(std::unique_ptr<ComponentType>(pComponent));
			m_EnabledComponents.PendPushBack(pComponent);

			return pComponent;
		}

		template<typename ComponentType, typename... Args, typename = std::enable_if<std::is_base_of_v<Component, ComponentType>>>
		ComponentType* AddComponent(Args&&... args)
		{
			return AddComponent(new ComponentType(std::forward<Args>(args)...));
		}

		template<typename ComponentType, typename = std::enable_if<std::is_base_of_v<Component, ComponentType>>>
		ComponentType* GetComponent() const
		{
			for (const auto& component : m_Components.GetVector())
				if (const auto result = dynamic_cast<ComponentType*>(component.get()); result != nullptr)
					return result;

			for (const auto& component : m_Components.GetElementsToAdd())
				if (const auto result = dynamic_cast<ComponentType*>(component.get()); result != nullptr)
					return result;

			return nullptr;
		}

		template<typename ComponentType, typename = std::enable_if<std::is_base_of_v<Component, ComponentType>>>
		ComponentType* GetComponentInChildren() const
		{
			for (const auto& child : m_Children)
			{
				if (ComponentType* pResult = child->GetComponent<ComponentType>(); pResult)
					return pResult;

				if (ComponentType* pResult = child->GetComponentInChildren<ComponentType>(); pResult)
					return pResult;
			}

			return nullptr;
		}

		void AddChild(GameObject* pChild);
		void RemoveChild(GameObject* pChild);

		Scene* GetScene() const;

		GameObject* GetParent() const;
		GameObject* GetRootParent();

		const DeferredVector<std::unique_ptr<GameObject>, GameObject*>& GetChildren();
		DeferredVector<GameObject*, GameObject*>& GetEnabledChildren();
		DeferredVector<GameObject*, GameObject*>& GetDisabledChildren();

		const GameObject* GetChildByName(const std::string& name) const;

		const DeferredVector<std::unique_ptr<Component>, Component*>& GetComponents();
		DeferredVector<Component*, Component*>& GetEnabledComponents();
		DeferredVector<Component*, Component*>& GetDisabledComponents();

		Transform& GetTransform();
		std::string GetName() const;

		bool HasTag(const std::string& tag) const;
		void AddTag(const std::string& tag);
		void RemoveTag(const std::string& tag);

	private:
		friend class Scene;
		
		void SetParent(GameObject* pParent);
		void SetScene(Scene* pScene);

		std::string m_Name{};

		GameObject* m_pParent{};
		Scene* m_pScene{};

		DeferredVector<std::unique_ptr<GameObject>, GameObject*> m_Children{};
		DeferredVector<GameObject*, GameObject*> m_EnabledChildren{};
		DeferredVector<GameObject*, GameObject*> m_DisabledChildren{};

		DeferredVector<std::unique_ptr<Component>, Component*> m_Components{};
		DeferredVector<Component*, Component*> m_EnabledComponents{};
		DeferredVector<Component*, Component*> m_DisabledComponents{};

		Transform m_Transform = Transform(this);

		std::set<std::string> m_Tags{};

		bool m_IsInitialized = false;
	};
}


#endif // Include Guard: GAMEOBJECT_H
