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
	explicit TankBulletComponent(TankComponent* pOwner, uint8_t maxBounces, float bulletSpeed, float damage);
private:
	void Start() override;
	void FixedUpdate() override;
	void OnOverlap(const CollisionResult&) override;

	const uint8_t m_MaxBounces;
	const float m_BulletSpeed;
	const float m_Damage;

	bool m_HitWallThisFrame = false;
	bool m_HitTankThisFrame = false;

	uint8_t m_CurrentBounces{};

	ProjectileMovementComponent* m_pProjectileMovement = nullptr;

	TankComponent* m_pOwningTank;
};


#endif // Include Guard: TANK_BULLET_COMPONENT_H
