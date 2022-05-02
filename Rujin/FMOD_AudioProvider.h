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
	struct ON_FMOD_CHANNELCONTROL_CALLBACK_END_USERDATA;

	enum class AudioChannel
	{
		SoundEffects,
		Music
	};

	class FMOD_AudioProvider final : public AudioService
	{
	public:
		FMOD_AudioProvider();
		~FMOD_AudioProvider() override;

		sound_id PlaySoundEffect(const std::string& filepath) override;
		sound_id PlayMusic(const std::string& filepath, bool loop = true) override;
		
		void StopAudio(sound_id id) override;
		void SetAudioPaused(const sound_id id, const bool isPaused) override;

		bool IsFinished(const sound_id id) override;
		bool IsPaused(const sound_id id) override;

		void LoadAudio(const std::string& filepath) override;
		void UnloadAudio(const std::string& filepath) override;

		void SetMusicVolume(float volume) override;
		void SetSoundEffectVolume(float volume) override;

		float GetMusicVolume() const override;
		float GetSoundEffectVolume() const override;

		

	private:
		sound_id PlayAudio(const std::string& filepath, AudioChannel channel);

		struct ActiveAudio
		{
			FMOD::Channel* pChannel;
			ON_FMOD_CHANNELCONTROL_CALLBACK_END_USERDATA* pCallbackData;
		};

		friend FMOD_RESULT F_CALL ON_FMOD_CHANNELCONTROL_CALLBACK_END(FMOD_CHANNELCONTROL* channelcontrol, FMOD_CHANNELCONTROL_TYPE controltype, FMOD_CHANNELCONTROL_CALLBACK_TYPE callbacktype, void* param1, void* param2);

		FMOD::Studio::System* m_pStudio = nullptr;
		FMOD::System* m_pCore = nullptr;

		FMOD::ChannelGroup* m_pSoundEffectChannel;
		FMOD::ChannelGroup* m_pMusicChannel;

		std::unordered_map<std::string, FMOD::Sound*> m_Sounds;


		std::unordered_map<sound_id, ActiveAudio> m_ActiveAudio;

		inline static size_t m_AudioInstanceCount{ 0 };
	};



	struct ON_FMOD_CHANNELCONTROL_CALLBACK_END_USERDATA
	{
		FMOD_AudioProvider* pAudioProvider;
		sound_id sound_id;
	};

	FMOD_RESULT F_CALL ON_FMOD_CHANNELCONTROL_CALLBACK_END(FMOD_CHANNELCONTROL* channelcontrol, FMOD_CHANNELCONTROL_TYPE controltype, FMOD_CHANNELCONTROL_CALLBACK_TYPE callbacktype, void* param1, void* param2);
}