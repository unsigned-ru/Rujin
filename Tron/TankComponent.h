#ifndef TANK_COMPONENT_H
#define TANK_COMPONENT_H

#include "Component.h"
#include "BoxColliderComponent.h"

namespace rujin
{
	class TextureRenderComponent;
}

class TankAimingComponent;

class TankMovementComponent;

class TankComponent final : public Component
{
public:
	explicit TankComponent(float maxHealth, TankMovementComponent* pTankMovement, TankAimingComponent* pTankAiming, TextureRenderComponent* pTankBodyRenderer, TextureRenderComponent* pTankTurretRenderer, BoxColliderComponent* pTankCollider);

	TankMovementComponent* GetMovement() const;
	TankAimingComponent* GetAiming() const;

	TextureRenderComponent* GetBodyRenderer() const;
	TextureRenderComponent* GetTurretRenderer() const;

	BoxColliderComponent* GetColliderComponent() const;

	void Shoot();

	void TakeDamage(float damage);

	void OnOverlap(const CollisionResult&) override;

private:
	TankMovementComponent* m_pTankMovement = nullptr;
	TankAimingComponent* m_pTankAiming = nullptr;

	TextureRenderComponent* m_pTankBodyRenderer = nullptr;
	TextureRenderComponent* m_pTankTurretRenderer = nullptr;

	BoxColliderComponent* m_pBoxCollider = nullptr;

	const uint8_t m_MaxBounces{ 5 };
	const float m_BulletSpeed{ 200.f };
	const float m_BulletDamage{ 25.f };

	const float m_MaxHealth;
	float m_CurrentHealth;
};

#endif // Include Guard: TANK_COMPONENT_H
