#pragma once
#include "AudioService.h"
#include <memory>

namespace rujin
{
	class Null_AudioProvider final : public AudioService
	{
	public:
		~Null_AudioProvider() override = default;
		void PlaySound(const std::shared_ptr<AudioClip>& clip, float volume) override {}
		void StopSound(const uint16_t id) override {}
		void StopAllSounds() override {}
	};
}
