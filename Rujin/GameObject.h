#pragma once
#include "Component.h"
#include "Transform.h"

#include "MonoBehaviour.h"

namespace rujin
{
	class GameObject final : public MonoBehaviour
	{
	public:
		explicit GameObject() = default;
		~GameObject() override = default;
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		void Start() override;
		void Update() override;
		void FixedUpdate() override;
		void Render() const override;

		template<typename ComponentType>
		std::weak_ptr<ComponentType> AddComponent()
		{
			if constexpr(!std::is_base_of<Component, ComponentType>())
				throw std::invalid_argument(std::string("Template type argument does not derive from Component."));

			auto& component = std::shared_ptr(new ComponentType());
			m_Components.push_back(component);
			return component;
		}

		template<typename ComponentType>
		std::weak_ptr<ComponentType> GetComponent() const
		{
			//TODO: implement, ask Laurens lol
		}

		void SetPosition(float x, float y);
		void SetParent(std::weak_ptr<GameObject> parent);

	private:
		Transform m_Transform{};
		std::weak_ptr<GameObject> m_Parent{};
		std::vector<std::weak_ptr<GameObject>> m_Children{};
		std::vector<std::shared_ptr<Component>> m_Components{};
	};
}
