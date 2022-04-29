#pragma once

namespace rujin
{
	class GameObject;
}

namespace rujin::event
{
	enum class Identifier
	{
		PlayerGainedPoints,
		PlayerDied
	};

	struct Data{};
	struct PlayerDiedData final : Data
	{
		explicit PlayerDiedData(GameObject* pPlayer, const uint16_t remainingLives)
			: m_pPlayer(pPlayer)
			, m_RemainingLives(remainingLives)
		{}

		GameObject* m_pPlayer;
		uint16_t m_RemainingLives;
	};


	struct PlayerGainedPointsData final : Data
	{
		explicit PlayerGainedPointsData(GameObject* pPlayer, const uint32_t newPoints)
			: m_pPlayer(pPlayer)
			, m_NewPoints(newPoints)
		{}

		GameObject* m_pPlayer;
		uint32_t m_NewPoints;
	};
}