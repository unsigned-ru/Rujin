#include "TronPCH.h"
#include "RecognizerComponent.h"

RecognizerComponent::RecognizerComponent
(
	TronMovementComponent* pMovement, TextureRenderComponent* pBodyRenderer, BoxColliderComponent* pCollider, 
	uint32_t attackDamage, uint32_t attackCooldown
)
	: m_pMovementComp(pMovement)
	, m_pBodyRenderer(pBodyRenderer)
	, m_pBoxCollider(pCollider)
	, m_AttackDamage(attackDamage)
	, m_AttackingCooldown(attackCooldown)
{
}

TronMovementComponent* RecognizerComponent::GetMovement() const { return m_pMovementComp; }
TextureRenderComponent* RecognizerComponent::GetBodyRenderer() const { return m_pBodyRenderer; }
BoxColliderComponent* RecognizerComponent::GetColliderComponent() const { return m_pBoxCollider; }

bool RecognizerComponent::CanAttack() const
{
	return std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - m_LastAttackTime).count() > m_AttackingCooldown;
}

void RecognizerComponent::Attack(rujin::GameObject* pObjectToAttack)
{
	pObjectToAttack;
	//TODO: reduce health
}
