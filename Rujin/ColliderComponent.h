#ifndef COLLIDER_COMPONENT_H
#define COLLIDER_COMPONENT_H

#include "Component.h"

namespace rujin
{
	class Collider;

	class ColliderComponent : public Component
	{
	public:
		explicit ColliderComponent(bool autoResolve = true);
		~ColliderComponent() override = default;

		ColliderComponent(const ColliderComponent&) = delete;
		ColliderComponent(ColliderComponent&&) noexcept = delete;
		ColliderComponent& operator=(const ColliderComponent&) = delete;
		ColliderComponent& operator=(ColliderComponent&&) noexcept = delete;

		virtual Collider* GetCollider() const = 0;

		void Start() override;
		void FixedUpdate() override;

		virtual void HandleCollision() = 0;

	protected:
		bool m_AutoResolve;
	};

}


#endif // Include Guard: COLLIDER_COMPONENT_H
