#include "TronPCH.h"
#include "TankComponent.h"

#include "GameObject.h"
#include "TankMovementComponent.h"
#include "TextureRenderComponent.h"


TankComponent::TankComponent(TankMovementComponent* pTankMovement, TankAimingComponent* pTankAiming, TextureRenderComponent* pTankBodyRenderer, TextureRenderComponent* pTankTurretRenderer, BoxColliderComponent* pTankCollider)
	: m_pTankMovement(pTankMovement)
	, m_pTankAiming(pTankAiming)
	, m_pTankBodyRenderer(pTankBodyRenderer)
	, m_pTankTurretRenderer(pTankTurretRenderer)
	, m_pBoxCollider(pTankCollider)
{
	ASSERT(m_pTankMovement, "This component requires a TankMovementComponent.");
	ASSERT(m_pTankAiming, "This component requires a TankAimingComponent.");
	ASSERT(m_pTankBodyRenderer, "This component requires a TextureRenderComponent for the body.");
	ASSERT(m_pBoxCollider, "This component requires a BoxColliderComponent.");

	m_pTankMovement->SetTank(this);
}

TankMovementComponent* TankComponent::GetMovement() const
{
	return m_pTankMovement;
}

TankAimingComponent* TankComponent::GetAiming() const
{
	return m_pTankAiming;
}

TextureRenderComponent* TankComponent::GetBodyRenderer() const
{
	return m_pTankBodyRenderer;
}

TextureRenderComponent* TankComponent::GetTurretRenderer() const
{
	return m_pTankTurretRenderer;
}

BoxColliderComponent* TankComponent::GetColliderComponent() const
{
	return m_pBoxCollider;
}
