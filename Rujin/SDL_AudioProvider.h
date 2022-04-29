#pragma once

#include "AudioService.h"

//forward declarations
namespace std { class thread; }

namespace rujin::services
{
	class SDL_AudioProvider final : public AudioService
	{
	public:
		SDL_AudioProvider();
		~SDL_AudioProvider() override;

		void PlaySound(const uint16_t id, const float volume) override;
		void StopSound(const uint16_t id) override;
		void StopAllSounds() override;


	private:
		void Update();

		std::thread* m_pAudioThread;
		std::atomic<bool> m_ExitThread;

		static constexpr uint16_t C_POLLING_RATE{ 50 }; // in milliseconds
		static constexpr uint16_t C_MAX_PENDING{ 50 }; // max pending sounds in 1 update


	};
}
