#include <isteamuser.h>
#include <isteamuserstats.h>
#include <map>

#include "IObserver.h"
#include "Singleton.h"

#define _ACH_ID( id, name ) std::make_pair(id, rujin::achievement::Data(id, std::string(#id).erase(0, std::string(#id).find_last_of(':') + 1), name, "", 0, 0))

namespace rujin
{
	namespace event
	{
		struct PlayerGainedPointsData;
	}

	namespace achievement
	{
		enum class Identifier
		{
			ACH_WIN_ONE_GAME = 0,
			ACH_WIN_100_GAMES = 1,
			ACH_TRAVEL_FAR_ACCUM = 2,
			ACH_TRAVEL_FAR_SINGLE = 3,
			NEW_ACHIEVEMENT_0_4 = 4
		};

		struct Data
		{
			explicit Data() = default;
			explicit Data(Identifier ID, const std::string& pchAchievementID, const std::string& rgchName, const std::string& rgchDescription, bool bAchieved, int iIconImage)
				: m_eAchievementID(ID), m_pchAchievementID(pchAchievementID), m_rgchName(rgchName), m_rgchDescription(rgchDescription), m_bAchieved(bAchieved), m_iIconImage(iIconImage) {}

			Identifier m_eAchievementID;
			const std::string m_pchAchievementID;
			std::string m_rgchName;
			std::string m_rgchDescription;
			bool m_bAchieved;
			int m_iIconImage;
		};

		class SteamAchievementSystem final : public Singleton<SteamAchievementSystem>, public event::IObserver
		{
		public:
			SteamAchievementSystem();
			~SteamAchievementSystem() override;

			bool RequestStats();
			bool SetAchievement(Identifier ID);

			STEAM_CALLBACK(SteamAchievementSystem, OnUserStatsReceived, UserStatsReceived_t, m_CallbackUserStatsReceived);
			STEAM_CALLBACK(SteamAchievementSystem, OnUserStatsStored, UserStatsStored_t, m_CallbackUserStatsStored);
			STEAM_CALLBACK(SteamAchievementSystem, OnAchievementStored, UserAchievementStored_t, m_CallbackAchievementStored);

		protected:
			void OnNotify(const event::Identifier identifier, const event::Data* pEventData) override;

		private:

			void OnPlayerGainedPoints(const event::PlayerGainedPointsData* pData);

			uint64_t m_iAppID; // Our current AppID
			std::map<Identifier, Data> m_Achievements;
			bool m_bInitialized; // Have we called Request stats and received the callback?
		};
	}

}


