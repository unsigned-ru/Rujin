#pragma once
#include <cstdint>

namespace rujin::services
{
	class AudioService
	{
	public:
		virtual ~AudioService() = default;
		virtual void PlaySound(const uint16_t id, const float volume) = 0;
		virtual void StopSound(const uint16_t id) = 0;
		virtual void StopAllSounds() = 0;
	};
}