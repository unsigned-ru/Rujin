#pragma once
#include "Service.h"

#include <cstdint>

namespace rujin
{
	using sound_id = size_t;

	class AudioService : public Service
	{
	public:
		virtual ~AudioService() override = default;

		virtual void LoadSound(const std::string& filepath) = 0;
		virtual void UnloadSound(const std::string& filepath) = 0;

		virtual sound_id PlaySound(const std::string& filepath, float volume) = 0;


		//TODO: playmusic
		//TODO: 
	};
}
