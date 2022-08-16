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
	explicit TankComponent
	(
		TankMovementComponent* pTankMovement,
		TankAimingComponent* pTankAiming,
		TextureRenderComponent* pTankBodyRenderer,
		TextureRenderComponent* pTankTurretRenderer,
		BoxColliderComponent* pTankCollider,
		const Recti& bulletSourceRect,
		float maxHealth = 100.f,
		float bulletSpeed = 300.f,
		float bulletDamage = 25.f,
		uint8_t bulletBounces = 5
	);

	TankMovementComponent* GetMovement() const;
	TankAimingComponent* GetAiming() const;

	TextureRenderComponent* GetBodyRenderer() const;
	TextureRenderComponent* GetTurretRenderer() const;

	BoxColliderComponent* GetColliderComponent() const;

	void Shoot();
	void TakeDamage(float damage);

private:
	TankMovementComponent* m_pTankMovement = nullptr;
	TankAimingComponent* m_pTankAiming = nullptr;

	TextureRenderComponent* m_pTankBodyRenderer = nullptr;
	TextureRenderComponent* m_pTankTurretRenderer = nullptr;

	BoxColliderComponent* m_pBoxCollider = nullptr;

	const Recti m_BulletSrcRect;

	const uint8_t m_BulletBounces;
	const float m_BulletSpeed;
	const float m_BulletDamage;

	const float m_MaxHealth;
	float m_CurrentHealth;
};

#endif // Include Guard: TANK_COMPONENT_H
