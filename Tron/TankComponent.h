#pragma once
#include "Component.h"

class TankAimingComponent;

namespace rujin
{
	class BoxColliderComponent;
	class TextureRenderComponent;
}

class TankMovementComponent;

class TankComponent final : public Component
{
public:
	explicit TankComponent(TankMovementComponent* pTankMovement, TankAimingComponent* pTankAiming, TextureRenderComponent* pTankBodyRenderer, TextureRenderComponent* pTankTurretRenderer, BoxColliderComponent* pTankCollider);

	TankMovementComponent* GetMovement() const;
	TankAimingComponent* GetAiming() const;

	TextureRenderComponent* GetBodyRenderer() const;
	TextureRenderComponent* GetTurretRenderer() const;

	BoxColliderComponent* GetColliderComponent() const;

private:
	TankMovementComponent* m_pTankMovement = nullptr;
	TankAimingComponent* m_pTankAiming = nullptr;

	TextureRenderComponent* m_pTankBodyRenderer = nullptr;
	TextureRenderComponent* m_pTankTurretRenderer = nullptr;

	BoxColliderComponent* m_pBoxCollider = nullptr;
};
