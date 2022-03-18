#pragma once

namespace rujin
{
	class GameObject;
}

namespace rujin::event
{
	enum class Identifier
	{
		PlayerDamaged,
		PlayerDied
	};

	struct Data{};
	struct PlayerDiedData final : Data
	{
		explicit PlayerDiedData(GameObject* pPlayer, const uint16_t& remainingLives)
			: m_pPlayer(pPlayer)
			, m_RemainingLives(remainingLives)
		{}

		GameObject* m_pPlayer;
		uint16_t m_RemainingLives;
	};

	
}