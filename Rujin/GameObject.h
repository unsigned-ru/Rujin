#pragma once
#include "Transform.h"
#include "SceneObject.h"

namespace dae
{
	class GameObject final
	{
	public:
		void Update();
		void FixedUpdate();
		void Render() const;

		void SetPosition(float x, float y);

		GameObject() = default;
		~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

	private:
		Transform m_Transform;
	};
}
