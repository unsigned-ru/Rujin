#ifndef GAME_EVENTS_H
#define GAME_EVENTS_H


#include "EngineEvents.h"

#include <cstdint>

class TronPlayerComponent;
class HealthComponent;

namespace game_event
{
	enum class Identifier : uint32_t
	{
		OnDamageTaken = static_cast<uint32_t>(event::Identifier::LAST_ENGINE_EVENT),
		OnHealingTaken,
		OnDie,
		OnScoreChanged,
		OnLivesChanged
	};

	struct OnDamageTaken_t final : event::Data
	{
		explicit OnDamageTaken_t(HealthComponent* pHealth, uint32_t damageDone, rujin::GameObject* pSource)
			: pHealth(pHealth)
			, damageDone(damageDone)
			, pSource(pSource)
		{}

		HealthComponent* pHealth;
		uint32_t damageDone;
		GameObject* pSource;
	};

	struct OnHealingTaken_t final : event::Data
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

	struct OnDie_t final : event::Data
	{
		explicit OnDie_t(HealthComponent* pHealth, rujin::GameObject* pKilledBy)
			: pHealth(pHealth)
			, pKilledBy(pKilledBy)
		{}

		HealthComponent* pHealth;
		rujin::GameObject* pKilledBy;
	};

	struct OnLivesChanged_t final : event::Data
	{
		explicit OnLivesChanged_t(TronPlayerComponent* pPlayer, uint8_t newLives)
			: pPlayer(pPlayer)
			, newLives(newLives)
		{}

		TronPlayerComponent* pPlayer;
		uint8_t newLives;
	};

	struct OnScoreChanged_t final : event::Data
	{
		explicit OnScoreChanged_t(TronPlayerComponent* pPlayer, uint32_t newScore)
			: pPlayer(pPlayer)
			, newScore(newScore)
		{}

		TronPlayerComponent* pPlayer;
		uint32_t newScore;
	};
}


#endif // Include Guard: GAME_EVENTS_H
