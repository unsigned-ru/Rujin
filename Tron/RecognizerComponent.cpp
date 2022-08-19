#include "TronPCH.h"
#include "RecognizerComponent.h"

#include "GameObject.h"
#include "HealthComponent.h"

RecognizerComponent::RecognizerComponent
(
	TronMovementComponent* pMovement, TextureRenderComponent* pBodyRenderer, BoxColliderComponent* pCollider, HealthComponent* pHealth,
	uint32_t attackDamage, uint32_t attackCooldown
)
	: m_pMovementComp(pMovement)
	, m_pBodyRenderer(pBodyRenderer)
	, m_pBoxCollider(pCollider)
	, m_pHealth(pHealth)
	, m_AttackDamage(attackDamage)
	, m_AttackingCooldown(attackCooldown)
{
}

TronMovementComponent* RecognizerComponent::GetMovement() const { return m_pMovementComp; }
TextureRenderComponent* RecognizerComponent::GetBodyRenderer() const { return m_pBodyRenderer; }
BoxColliderComponent* RecognizerComponent::GetColliderComponent() const { return m_pBoxCollider; }
HealthComponent* RecognizerComponent::GetHealthComponent() const { return m_pHealth;  }

bool RecognizerComponent::CanAttack() const
{
	return std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - m_LastAttackTime).count() > m_AttackingCooldown;
}

void RecognizerComponent::Attack(const rujin::GameObject* pObjectToAttack)
{
	if (!CanAttack())
		return;

	if (HealthComponent* pHealth = pObjectToAttack->GetComponent<HealthComponent>(); pHealth)
	{
		pHealth->TakeDamage(m_AttackDamage, GameObject());
	}

	m_LastAttackTime = std::chrono::high_resolution_clock::now();
}
