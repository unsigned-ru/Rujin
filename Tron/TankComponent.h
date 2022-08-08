#pragma once
#include "Component.h"

namespace rujin
{
	class BoxColliderComponent;
	class TextureRenderComponent;
}

class TankMovementComponent;

class TankComponent final : public Component
{
public:
	explicit TankComponent(TankMovementComponent* pTankMovement, TextureRenderComponent* pTankBodyRenderer, TextureRenderComponent* pTankTurretRenderer, BoxColliderComponent* pTankCollider);

	TankMovementComponent* GetMovement() const;
	TextureRenderComponent* GetBodyRenderer() const;
	TextureRenderComponent* GetTurretRenderer() const;
	BoxColliderComponent* GetColliderComponent() const;

private:
	TankMovementComponent* m_pTankMovement = nullptr;
	TextureRenderComponent* m_pTankBodyRenderer = nullptr;
	TextureRenderComponent* m_pTankTurretRenderer = nullptr;
	BoxColliderComponent* m_pBoxCollider = nullptr;

};
