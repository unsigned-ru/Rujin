#pragma once
#include "Component.h"
#include "IGameLoopObject.h"
#include "TransformComponent.h"

namespace rujin
{
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
		void Update() override;
		void FixedUpdate() override;
		void OnGui(SDL_Window* pWindow) override;
		void Render() const override;
		void Destroy() override;

		template<typename ComponentType, typename = std::enable_if<std::is_base_of_v<Component, ComponentType>>>
		ComponentType* AddComponent(ComponentType* component)
		{
			auto ptr = std::unique_ptr<ComponentType>(component);
			ptr->m_pGameObject = this;
			m_Components.push_back(std::move(ptr));
			return component;
		}

		template<typename ComponentType, typename = std::enable_if<std::is_base_of_v<Component, ComponentType>>>
		ComponentType* AddComponent()
		{
			return AddComponent(new ComponentType());
		}

		template<typename ComponentType, typename = std::enable_if<std::is_base_of_v<Component, ComponentType>>>
		ComponentType* GetComponent() const
		{
			for (const auto& component : m_Components)
				if (const auto result = dynamic_cast<ComponentType*>(component.get()); result != nullptr)
					return result;

			return nullptr;
		}

		void SetParent(GameObject* pParent);

		GameObject* GetParent() const;
		TransformComponent* GetTransform() const;
		std::string GetName() const;

	private:
		std::string m_Name{};

		GameObject* m_pParent{ nullptr }; //Note owned by GameObject
		std::vector<std::unique_ptr<GameObject>> m_Children{}; //owned by GameObject
		std::vector<std::unique_ptr<Component>> m_Components{}; //owned by GameObject

		// all GO's have a transform so we can speed up access time by storing it explicitly
		// gets destroyed by unique_ptr in m_Components going out of scope
		TransformComponent* m_pTransformComp; 
	};
}
