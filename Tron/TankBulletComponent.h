#ifndef TANK_BULLET_COMPONENT_H
#define TANK_BULLET_COMPONENT_H
#include "Component.h"
#include "IObserver.h"


class TankComponent;

namespace rujin
{
	class ProjectileMovementComponent;
}

class TankBulletComponent final : public Component, public event::IObserver
{
public:
	explicit TankBulletComponent(TankComponent* pOwner, uint8_t maxBounces, float bulletSpeed, uint32_t damage);

private:
	void Start() override;
	void FixedUpdate() override;
	void OnOverlap(const CollisionResult&) override;

	void OnNotify(const uint32_t identifier, const event::Data* pEventData) override;

	const uint8_t m_MaxBounces;
	const float m_BulletSpeed;
	const uint32_t m_Damage;

	uint8_t m_CurrentBounces{};


	TankComponent* m_pOwnerTank = nullptr;
	ProjectileMovementComponent* m_pProjectileMovement = nullptr;

	bool m_HitEnemyThisFrame{ false };
	bool m_HitWallThisFrame{ false };
};


#endif // Include Guard: TANK_BULLET_COMPONENT_H
