#ifndef PROJECTILE_MOVEMENT_COMPONENT_H
#define PROJECTILE_MOVEMENT_COMPONENT_H


#include "Component.h"

namespace rujin
{
	class ProjectileMovementComponent final : public Component
	{
	public:
		explicit ProjectileMovementComponent() = default;
		explicit ProjectileMovementComponent(const glm::vec2& initialVelocity);

		void FixedUpdate() override;

		void SetVelocity(const glm::vec2& velocity);
		const glm::vec2& GetVelocity() const;

	private:
		glm::vec2 m_Velocity{1.f, 0.f};
	};

}


#endif // Include Guard: PROJECTILE_MOVEMENT_COMPONENT_H
