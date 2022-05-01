#pragma once

#include <functional>
#include <mutex>

#include "AudioService.h"

//forward declarations
namespace std { class thread; }

namespace rujin
{
	class SDL_AudioProvider final : public AudioService
	{
	public:
		SDL_AudioProvider();
		~SDL_AudioProvider() override;

		sound_id PlaySound(const std::string& filepath, float volume) override;

		void LoadSound(const std::string& filepath) override;
		void UnloadSound(const std::string& filepath) override;

	private:
		void RunThread();
	private:
		std::thread* m_pAudioThread;
		std::atomic<bool> m_ExitThread;
		std::mutex m_QueueMutex;

		/*std::vector<AudioRequest> m_AudioQueue;*/
		
		static constexpr uint16_t C_POLLING_RATE{ 50 }; // in milliseconds
	};
}