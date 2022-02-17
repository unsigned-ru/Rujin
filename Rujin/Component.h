#pragma once
#include "MonoBehaviour.h"

namespace rujin
{
	class GameObject;

	class Component : public MonoBehaviour
	{
	public:
		explicit Component(const std::weak_ptr<GameObject> gameObject);
		~Component() override = default;
		Component(const Component& other) = delete;
		Component(Component&& other) = delete;
		Component& operator=(const Component& other) = delete;
		Component& operator=(Component&& other) = delete;

		void Start() override;
		void Update() override;
		void FixedUpdate() override;
		void Render() const override;
		void Destroy() override;

		std::weak_ptr<GameObject> GetGameObject() const;

	protected:
		std::weak_ptr<GameObject> m_GameObject{};
	};

}

