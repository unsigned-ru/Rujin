#ifndef TANK_COMPONENT_H
#define TANK_COMPONENT_H

#include <chrono>

#include "Component.h"
#include "BoxColliderComponent.h"

namespace rujin
{
	class TextureRenderComponent;
}

class TankAimingComponent;
class TronMovementComponent;


class TankComponent final : public Component
{
public:
	explicit TankComponent
	(
		TronMovementComponent* pTankMovement,
		TankAimingComponent* pTankAiming,
		TextureRenderComponent* pTankBodyRenderer,
		TextureRenderComponent* pTankTurretRenderer,
		BoxColliderComponent* pTankCollider,
		const Recti& bulletSourceRect,
		float bulletSpeed = 300.f,
		float bulletDamage = 25.f,
		uint32_t shootingCooldown = 1'000u,
		uint8_t bulletBounces = 5u
	);

	TronMovementComponent* GetMovement() const;
	TankAimingComponent* GetAiming() const;

	TextureRenderComponent* GetBodyRenderer() const;
	TextureRenderComponent* GetTurretRenderer() const;

	BoxColliderComponent* GetColliderComponent() const;

	bool CanShoot() const;
	void Shoot();

private:
	TronMovementComponent* m_pMovement = nullptr;
	TankAimingComponent* m_pAiming = nullptr;

	TextureRenderComponent* m_pBodyRenderer = nullptr;
	TextureRenderComponent* m_pTurretRenderer = nullptr;

	BoxColliderComponent* m_pBodyCollider = nullptr;

	const Recti m_BulletSrcRect;

	const uint8_t m_BulletBounces;
	const float m_BulletSpeed;
	const float m_BulletDamage;

	const uint32_t m_ShootingCooldown;

	using TimePoint = std::chrono::time_point<std::chrono::steady_clock>;
	TimePoint m_LastShotTime = std::chrono::high_resolution_clock::now();
};

#endif // Include Guard: TANK_COMPONENT_H
