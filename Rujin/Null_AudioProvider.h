#pragma once
#include "AudioService.h"
#include <memory>

namespace rujin
{
	class Null_AudioProvider final : public AudioService
	{
	public:
		Null_AudioProvider() = default;
		~Null_AudioProvider() override = default;

		Null_AudioProvider(const Null_AudioProvider&) = delete;
		Null_AudioProvider(Null_AudioProvider&&) noexcept = delete;
		Null_AudioProvider& operator=(const Null_AudioProvider&) = delete;
		Null_AudioProvider& operator=(Null_AudioProvider&&) noexcept = delete;

		void PlaySound(const std::shared_ptr<AudioClip>& clip, float volume) override {}
		void StopSound(const uint16_t id) override {}
		void StopAllSounds() override {}
	};
}
