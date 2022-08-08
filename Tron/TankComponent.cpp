#include "TronPCH.h"
#include "TankComponent.h"

#include "TankMovementComponent.h"
#include "TextureRenderComponent.h"


TankComponent::TankComponent(TankMovementComponent* pTankMovement, TextureRenderComponent* pTankBodyRenderer, TextureRenderComponent* pTankTurretRenderer, BoxColliderComponent* pTankCollider)
	: m_pTankMovement(pTankMovement)
	, m_pTankBodyRenderer(pTankBodyRenderer)
	, m_pTankTurretRenderer(pTankTurretRenderer)
	, m_pBoxCollider(pTankCollider)
{
	ASSERT(pTankMovement, "This component requires a TankMovementComponent.");
	ASSERT(pTankBodyRenderer, "This component requires a TextureRenderComponent for the body.");
	ASSERT(pTankCollider, "This component requires a BoxColliderComponent.");

	m_pTankMovement->SetTank(this);
}

TankMovementComponent* TankComponent::GetMovement() const
{
	return m_pTankMovement;
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
