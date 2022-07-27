#pragma once
#include "Service.h"

#include <cstdint>

namespace rujin
{
	using sound_id = int32_t;

	class AudioService : public Service
	{
	public:
		AudioService() = default;
		~AudioService() override = default;

		AudioService(const AudioService&) = delete;
		AudioService(AudioService&&) noexcept = delete;
		AudioService& operator=(const AudioService&) = delete;
		AudioService& operator=(AudioService&&) noexcept = delete;

		virtual void LoadAudio(const std::string& filepath) = 0;
		virtual void UnloadAudio(const std::string& filepath) = 0;

		virtual sound_id PlaySoundEffect(const std::string& filepath) = 0;
		virtual sound_id PlayMusic(const std::string& filepath, bool loop = true) = 0;

		virtual void StopAudio(const sound_id id) = 0;
		virtual void SetAudioPaused(const sound_id id, const bool isPaused) = 0;

		virtual bool IsFinished(const sound_id id) = 0;
		virtual bool IsPaused(const sound_id id) = 0;

		virtual void SetMusicVolume(float volume) = 0;
		virtual void SetSoundEffectVolume(float volume) = 0;

		virtual float GetMusicVolume() const = 0;
		virtual float GetSoundEffectVolume() const = 0;
	};
}
