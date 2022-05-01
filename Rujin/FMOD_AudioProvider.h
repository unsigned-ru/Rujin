#pragma once
#include "AudioService.h"

#pragma warning(push)
#pragma warning(disable: 4505 26812)
#include <unordered_map>

#include "fmod_studio.hpp" //Precompiled Header
#include "fmod_errors.h"
#pragma warning(pop)

#if defined(DEBUG) || defined(_DEBUG)
#pragma comment(lib, "fmodL_vc.lib")
#else
#pragma comment(lib, "fmod_vc.lib")
#endif
#pragma endregion

namespace rujin
{
	class FMOD_AudioProvider final : public AudioService
	{
	public:
		FMOD_AudioProvider();
		~FMOD_AudioProvider() override;

		sound_id PlaySound(const std::string& filepath, float volume) override;

		void LoadSound(const std::string& filepath) override;
		void UnloadSound(const std::string& filepath) override;

	private:
		FMOD::Studio::System* m_pStudio = nullptr;
		FMOD::System* m_pCore = nullptr;

		std::unordered_map<std::string, FMOD::Sound*> m_Sounds;
	};
}
