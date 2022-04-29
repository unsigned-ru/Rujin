#include "RujinPCH.h"

#include "SteamAchievementSystem.h"

#include <isteamutils.h>

#include "GameEvents.h"

rujin::achievement::SteamAchievementSystem::SteamAchievementSystem()
	: m_iAppID(SteamUtils()->GetAppID())
    , m_bInitialized(false)
    , m_CallbackUserStatsReceived(this, &SteamAchievementSystem::OnUserStatsReceived)
    , m_CallbackUserStatsStored(this, &SteamAchievementSystem::OnUserStatsStored)
    , m_CallbackAchievementStored(this, &SteamAchievementSystem::OnAchievementStored)
	, m_Achievements
	{
		_ACH_ID(Identifier::ACH_WIN_ONE_GAME, "Winner"),
		_ACH_ID(Identifier::ACH_WIN_100_GAMES, "Champion"),
		_ACH_ID(Identifier::ACH_TRAVEL_FAR_ACCUM, "Interstellar"),
		_ACH_ID(Identifier::ACH_TRAVEL_FAR_SINGLE, "Orbiter"),
		_ACH_ID(Identifier::NEW_ACHIEVEMENT_0_4, "NEW_ACHIEVEMENT_0_4")
	}
{
    RequestStats();
}

rujin::achievement::SteamAchievementSystem::~SteamAchievementSystem() = default;

void rujin::achievement::SteamAchievementSystem::OnNotify(const event::Identifier identifier, const event::Data* pEventData)
{

	switch (identifier)
	{
	case event::Identifier::PlayerGainedPoints:
		OnPlayerGainedPoints(static_cast<const event::PlayerGainedPointsData* const>(pEventData));
		break;

	default: break;
	}
}

void rujin::achievement::SteamAchievementSystem::OnPlayerGainedPoints(const event::PlayerGainedPointsData* pData)
{
	//if the player has no remaining lives, give them the achievement
	if (pData->m_NewPoints >= 500)
	{
		if (!m_Achievements.at(Identifier::ACH_WIN_ONE_GAME).m_bAchieved)
			SetAchievement(Identifier::ACH_WIN_ONE_GAME);
	}
}


bool rujin::achievement::SteamAchievementSystem::RequestStats()
{
	// Is Steam loaded? If not we can't get stats.
	if (!SteamUserStats() || !SteamUser())
		return false;

	// Is the user logged on?  If not we can't get stats.
	if (!SteamUser()->BLoggedOn())
		return false;

	// Request user stats.
	return SteamUserStats()->RequestCurrentStats();
}
bool rujin::achievement::SteamAchievementSystem::SetAchievement(Identifier ID)
{
	// Have we received a call back from Steam yet?
	if (m_bInitialized)
	{
		SteamUserStats()->SetAchievement(m_Achievements[ID].m_pchAchievementID.c_str());
		return SteamUserStats()->StoreStats();
	}
	// If not then we can't set achievements yet
	return false;
}
void rujin::achievement::SteamAchievementSystem::OnUserStatsReceived(UserStatsReceived_t* pCallback)
{
	// we may get callbacks for other games' stats arriving, ignore them
	if (m_iAppID == pCallback->m_nGameID)
	{
		if (pCallback->m_eResult == k_EResultOK)
		{
			OutputDebugString("Received stats and achievements from Steam\n");
			m_bInitialized = true;

			// load achievements
			for (auto& pair : m_Achievements)
			{
				Data& ach = pair.second;
				SteamUserStats()->GetAchievement(ach.m_pchAchievementID.c_str(), &ach.m_bAchieved);

				ach.m_rgchName = SteamUserStats()->GetAchievementDisplayAttribute(ach.m_pchAchievementID.c_str(), "name");
				ach.m_rgchDescription = SteamUserStats()->GetAchievementDisplayAttribute(ach.m_pchAchievementID.c_str(), "desc");
			}
		}
		else
		{
			char buffer[128];
			_snprintf_s(buffer, 128, "RequestStats - failed, %d\n", pCallback->m_eResult);
			OutputDebugString(buffer);
		}
	}
}
void rujin::achievement::SteamAchievementSystem::OnUserStatsStored(UserStatsStored_t* pCallback)
{
	// we may get callbacks for other games' stats arriving, ignore them
	if (m_iAppID == pCallback->m_nGameID)
	{
		if (k_EResultOK == pCallback->m_eResult)
		{
			OutputDebugString("Stored stats for Steam\n");
		}
		else
		{
			char buffer[128];
			_snprintf_s(buffer, 128, "StatsStored - failed, %d\n", pCallback->m_eResult);
			OutputDebugString(buffer);
		}
	}
}
void rujin::achievement::SteamAchievementSystem::OnAchievementStored(UserAchievementStored_t* pCallback)
{
	// we may get callbacks for other games' stats arriving, ignore them
	if (m_iAppID == pCallback->m_nGameID)
	{
		OutputDebugString("Stored Achievement for Steam\n");
	}
}

