#ifndef TANK_BULLET_COMPONENT_H
#define TANK_BULLET_COMPONENT_H
#include "Component.h"


class TankComponent;

namespace rujin
{
	class ProjectileMovementComponent;
}

class TankBulletComponent final : public Component
{
public:
	explicit TankBulletComponent(TankComponent* pOwner, uint8_t maxBounces, float bulletSpeed, uint32_t damage);
private:
	void Start() override;
	void FixedUpdate() override;
	void OnOverlap(const CollisionResult&) override;

	const uint8_t m_MaxBounces;
	const float m_BulletSpeed;
	const uint32_t m_Damage;

	uint8_t m_CurrentBounces{};

	ProjectileMovementComponent* m_pProjectileMovement = nullptr;

	TankComponent* m_pOwningTank;

	bool m_HasHitPlayerThisFrame{ false };
};


#endif // Include Guard: TANK_BULLET_COMPONENT_H
