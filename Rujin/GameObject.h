#pragma once
#include "Component.h"
#include "MonoBehaviour.h"
#include "TransformComponent.h"

namespace rujin
{
	/**
	 * \brief Uses shared_ptr only policy.
	 * \note GameObjectFactory handles instantiation.
	*/
	class GameObject final : public MonoBehaviour, public std::enable_shared_from_this<GameObject>
	{
	public:
		~GameObject() override = default;
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		void Start() override;
		void Update() override;
		void FixedUpdate() override;
		void Render() const override;
		void Destroy() override;

		template<typename ComponentType, typename = std::enable_if<std::is_base_of_v<Component, ComponentType>>>
		std::weak_ptr<ComponentType> AddComponent()
		{
			auto component = std::make_shared<ComponentType>(weak_from_this());
			m_Components.push_back(component);
			return component;
		}

		template<typename ComponentType, typename = std::enable_if<std::is_base_of_v<Component, ComponentType>>>
		std::weak_ptr<ComponentType> GetComponent() const
		{
			for (const auto& component : m_Components)
				if (const auto result = std::dynamic_pointer_cast<ComponentType>(component); result != nullptr)
					return result;

			return std::weak_ptr<ComponentType>();
		}

		std::weak_ptr<TransformComponent> GetTransform();

		void SetParent(std::weak_ptr<GameObject> parent);
		std::weak_ptr<GameObject> GetParent() const;

		std::string GetName() const;
		explicit GameObject(const std::string& name);
	private:
		/*friend class GameObjectFactory;
		explicit GameObject(const std::string& name);*/

		std::string m_Name{};

		std::weak_ptr<GameObject> m_Parent{};
		std::vector<std::weak_ptr<GameObject>> m_Children{};
		std::vector<std::shared_ptr<Component>> m_Components{};
	};
}
