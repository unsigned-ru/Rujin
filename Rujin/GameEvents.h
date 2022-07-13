#pragma once
#include "EventData.h"
#include <stdint.h>

enum class EventIdentifier : uint32_t
{
};

class GameObject;
using namespace rujin;

//struct PlayerDiedEventData final : event::Data
//{
//	explicit PlayerDiedData(GameObject* pPlayer, const uint16_t remainingLives)
//		: m_pPlayer(pPlayer)
//		, m_RemainingLives(remainingLives)
//	{}
//
//	GameObject* m_pPlayer;
//	uint16_t m_RemainingLives;
//};
//
//
//struct PlayerGainedPointsData final : event::Data
//{
//	explicit PlayerGainedPointsData(GameObject* pPlayer, const uint32_t newPoints)
//		: m_pPlayer(pPlayer)
//		, m_NewPoints(newPoints)
//	{}
//
//	GameObject* m_pPlayer;
//	uint32_t m_NewPoints;
//};