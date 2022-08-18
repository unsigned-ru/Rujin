#ifndef HEALTH_COMPONENT_H
#define HEALTH_COMPONENT_H
#include "Component.h"
#include "Subject.h"


class HealthComponent : public Component, public event::Subject
{
public:
	explicit HealthComponent(uint32_t maxHealth);
	explicit HealthComponent(uint32_t maxHealth, uint32_t currentHealth);

	/**
	 * \brief Take damage...
	 * \param damage damage to take
	 * \note Triggers <c>OnDamageTaken</c> event.
	 */
	void TakeDamage(uint32_t damage, class GameObject* pSource);
	/**
	 * \brief take healing...
	 * \note Triggers <c>OnHealingTaken</c> event.
	 * \param healing to take
	 */
	void TakeHealing(uint32_t healing, class GameObject* pSource);
	/**
	 * \brief Set current health.
	 * \param newHealth new current health to set.
	 * \note This does not trigger any events.
	 */
	void SetHealth(uint32_t newHealth);

	uint32_t GetMaxHealth() const;
	uint32_t GetCurrentHealth() const;

	bool IsDead() const;

private:
	const uint32_t m_MaxHealth{};
	uint32_t m_CurrentHealth{ m_MaxHealth };


};


#endif // Include Guard: HEALTH_COMPONENT_H
