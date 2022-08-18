#ifndef GAME_EVENTS_H
#define GAME_EVENTS_H


#include "EngineEvents.h"
#include "HealthComponent.h"

#include <cstdint>

namespace game_event
{
	enum class Identifier : uint32_t
	{
		OnDamageTaken = static_cast<uint32_t>(rujin::event::Identifier::LAST_ENGINE_EVENT),
		OnHealingTaken,
		OnDie
	};

	struct OnDamageTaken_t final : rujin::event::Data
	{
		explicit OnDamageTaken_t(HealthComponent* pHealth, uint32_t damageDone, rujin::GameObject* pSource)
			: pHealth(pHealth)
			, damageDone(damageDone)
			, pSource(pSource)
		{}

		HealthComponent* pHealth;
		uint32_t damageDone;
		rujin::GameObject* pSource;
	};

	struct OnHealingTaken_t final : rujin::event::Data
	{
		explicit OnHealingTaken_t(HealthComponent* pHealth, uint32_t healingDone, rujin::GameObject* pSource)
			: pHealth(pHealth)
			, healingDone(healingDone)
			, pSource(pSource)
		{}

		HealthComponent* pHealth;
		uint32_t healingDone;
		rujin::GameObject* pSource;
	};

	struct OnDie_t final : rujin::event::Data
	{
		explicit OnDie_t(HealthComponent* pHealth, rujin::GameObject* pKilledBy)
			: pHealth(pHealth)
			, pKilledBy(pKilledBy)
		{}

		HealthComponent* pHealth;
		rujin::GameObject* pKilledBy;
	};
}


#endif // Include Guard: GAME_EVENTS_H
