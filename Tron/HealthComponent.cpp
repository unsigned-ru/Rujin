#include "TronPCH.h"
#include "HealthComponent.h"

#include "GameEvents.h"

HealthComponent::HealthComponent(uint32_t maxHealth)
	: HealthComponent(maxHealth, maxHealth)
{}

HealthComponent::HealthComponent(uint32_t maxHealth, uint32_t currentHealth)
	: Subject({})
	, m_MaxHealth(maxHealth)
	, m_CurrentHealth(currentHealth)
{}

void HealthComponent::TakeDamage(uint32_t damage, class GameObject* pSource)
{
	//don't take damage if we're already dead
	if (IsDead())
		return;

	//store old health for event
	const uint32_t oldHealth = m_CurrentHealth;
	//take damage
	m_CurrentHealth = std::clamp(m_CurrentHealth - damage, 0u, m_MaxHealth);

	//notify damage
	const auto damageTakenEvent = game_event::OnDamageTaken_t(this, oldHealth - m_CurrentHealth, pSource);

	Notify
	(
		static_cast<uint32_t>(game_event::Identifier::OnDamageTaken),
		&damageTakenEvent
	);

	if (IsDead())
	{
		const auto dieEvent = game_event::OnDie_t(this, pSource);

		//we died.
		//notify death.
		Notify
		(
			static_cast<uint32_t>(game_event::Identifier::OnDie),
			&dieEvent
		);
	}
}

void HealthComponent::TakeHealing(uint32_t healing, class GameObject* pSource)
{
	//if we are dead, we can't be healed. We need to be ressurected.
	if (IsDead())
		return;

	const uint32_t oldHealth = m_CurrentHealth;
	m_CurrentHealth = std::clamp(m_CurrentHealth + healing, 0u, m_MaxHealth);

	const auto eventData = game_event::OnHealingTaken_t(this, m_CurrentHealth - oldHealth, pSource);

	Notify
	(
		static_cast<uint32_t>(game_event::Identifier::OnHealingTaken),
		&eventData
	);
}

void HealthComponent::SetHealth(uint32_t newHealth)
{
	m_CurrentHealth = newHealth;
}

uint32_t HealthComponent::GetMaxHealth() const { return m_MaxHealth; }

uint32_t HealthComponent::GetCurrentHealth() const { return m_CurrentHealth; }

bool HealthComponent::IsDead() const
{
	return m_CurrentHealth == 0;
}
