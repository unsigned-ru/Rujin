#pragma once

#include <functional>
#include <mutex>

#include "AudioService.h"

//forward declarations
namespace std { class thread; }

namespace rujin
{
	/**
	 * \brief only supports playing sound effects, made this to demonstrate that i can create a audio thread and event-queue driven audio system
	 */
	class SDL_AudioProvider final : public AudioService
	{
	public:
		SDL_AudioProvider();
		~SDL_AudioProvider() override;

		sound_id PlaySoundEffect(const std::string& filepath) override;
		sound_id PlayMusic(const std::string&, bool) override { return 0; };

		void StopAudio(const sound_id) override {};
		void SetAudioPaused(const sound_id, const bool) override {};

		bool IsFinished(const sound_id) override { return true; };
		bool IsPaused(const sound_id) override { return true; };

		void SetMusicVolume(float) override {};
		void SetSoundEffectVolume(float) override {};

		float GetMusicVolume() const override { return 100; };
		float GetSoundEffectVolume() const override { return 100; };

		void LoadAudio(const std::string& filepath) override;
		void UnloadAudio(const std::string& filepath) override;

	private:
		void RunThread();
		void SwapQueueBuffers();

		struct AudioRequest
		{
			const std::string filepath;
		};


		std::thread* m_pAudioThread{ nullptr };
		std::atomic<bool> m_ExitThread{ false };
		
		std::vector<AudioRequest> m_FillQueue{};
		std::vector<AudioRequest> m_ThreadQueue{};
		std::mutex m_QueueMutex{};


		std::unordered_map<std::string, Mix_Chunk*> m_Audio{};

		static constexpr uint16_t C_POLLING_RATE{ 50 }; // in milliseconds
	};
}

