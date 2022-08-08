#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "IGameLoopObject.h"
#include "TransformComponent.h"

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
	class GameObject final : public IGameLoopObject
	{
	public:
		explicit GameObject(const std::string& name);
		~GameObject() override = default;
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		void Start() override;
		void LateStart() override;
		void Update() override;
		void FixedUpdate() override;
		void OnGui(SDL_Window* pWindow) override;
		void Draw() const override;
		void Destroy() override;

		void BroadcastOverlap(const CollisionResult& collision);

		template<typename ComponentType, typename = std::enable_if<std::is_base_of_v<Component, ComponentType>>>
		ComponentType* AddComponent(ComponentType* component)
		{
			auto ptr = std::unique_ptr<ComponentType>(component);
			ptr->m_pGameObject = this;
			m_Components.push_back(std::move(ptr));
			return component;
		}

		template<typename ComponentType, typename... Args, typename = std::enable_if<std::is_base_of_v<Component, ComponentType>>>
		ComponentType* AddComponent(Args&&... args)
		{
			return AddComponent(new ComponentType(std::forward<Args>(args)...));
		}

		template<typename ComponentType, typename = std::enable_if<std::is_base_of_v<Component, ComponentType>>>
		ComponentType* GetComponent() const
		{
			for (const auto& component : m_Components)
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

		void AddChild(std::unique_ptr<GameObject>& pChild);
		void AttachTo(GameObject* pParent);

		Scene* GetScene() const;
		GameObject* GetParent() const;
		GameObject* GetRootParent();
		const std::vector<std::unique_ptr<GameObject>>& GetChildren() const;
		const GameObject* GetChildByName(const std::string& name) const;
		const std::vector<std::unique_ptr<Component>>& GetComponents() const;
		TransformComponent* GetTransform() const;
		std::string GetName() const;

	private:
		friend class Scene;
		
		void SetParent(GameObject* pParent);
		void SetScene(Scene* pScene);

		std::string m_Name{};

		GameObject* m_pParent{};
		Scene* m_pScene{};
		std::vector<std::unique_ptr<GameObject>> m_Children{};
		std::vector<std::unique_ptr<Component>> m_Components{};

		// all GO's have a transform so we can speed up access time by storing it explicitly
		// gets destroyed by unique_ptr in m_Components going out of scope
		TransformComponent* m_pTransformComp; 
	};
}


#endif // Include Guard: GAMEOBJECT_H
