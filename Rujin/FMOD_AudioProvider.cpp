#include "RujinPCH.h"
#include "FMOD_AudioProvider.h"

#include <functional>
#include <ranges>

#include "ResourceService.h"
#include "ServiceLocator.h"
#include "Rutils/Macros.h"

#define LOG_FMOD_ERROR(result) LOG_ERROR_("FMOD Error! {} {}", result, FMOD_ErrorString(result))
#define HANDLE_FMOD_ERROR(result) {auto fmod_result = result; if ((fmod_result) != FMOD_OK) LOG_FMOD_ERROR(fmod_result);}

rujin::FMOD_AudioProvider::FMOD_AudioProvider()
{
	FMOD::Debug_Initialize(FMOD_DEBUG_LEVEL_ERROR);

	unsigned int version{};
	int numdrivers{};

	//Create studio system, and store core and studio
	HANDLE_FMOD_ERROR(FMOD::Studio::System::create(&m_pStudio));
	HANDLE_FMOD_ERROR(m_pStudio->getCoreSystem(&m_pCore));

	//assert version
	HANDLE_FMOD_ERROR(m_pCore->getVersion(&version))
	if (version < FMOD_VERSION)
		LOG_ERROR_("Initialization Failed!\n\nYou are using an old version of FMOD {:#x}. This program requires {:#x}\n", version, FMOD_VERSION);

	HANDLE_FMOD_ERROR(m_pCore->getNumDrivers(&numdrivers));

	if (numdrivers == 0)
	{
		HANDLE_FMOD_ERROR(m_pCore->setOutput(FMOD_OUTPUTTYPE_NOSOUND));
	}
	else
	{
		HANDLE_FMOD_ERROR(m_pStudio->initialize(32, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, nullptr))
	}

	//Create Music and SFX channels
	HANDLE_FMOD_ERROR(m_pCore->createChannelGroup("Sound Effects", &m_pSoundEffectChannel));
	HANDLE_FMOD_ERROR(m_pCore->createChannelGroup("Music", &m_pMusicChannel));
}

rujin::FMOD_AudioProvider::~FMOD_AudioProvider()
{
	//release sounds
	for (auto* pSound : m_Sounds | std::views::values)
	{
		pSound->release();
	}

	if (m_pStudio)
	{
#pragma warning(push)
#pragma warning(disable : 26812)
		//note: Releases core too.
		m_pStudio->release();

		m_pStudio = nullptr;
		m_pCore = nullptr;
#pragma warning(pop)
	}

	//release all callback data to avoid memory leaks
	for (const auto& activeAudio : m_ActiveAudio | std::views::values)
	{
		delete activeAudio.pCallbackData;
	}
	m_ActiveAudio.clear();

}

rujin::sound_id rujin::FMOD_AudioProvider::PlaySoundEffect(const std::string& filepath)
{
	return PlayAudio(filepath, AudioChannel::SoundEffects);
}

rujin::sound_id rujin::FMOD_AudioProvider::PlayMusic(const std::string& filepath, bool loop)
{
	const auto id = PlayAudio(filepath, AudioChannel::Music);

	if (loop) 
		m_ActiveAudio.at(id).pChannel->setLoopCount(-1);

	return id;
}

void rujin::FMOD_AudioProvider::StopAudio(sound_id id)
{
	const auto it = m_ActiveAudio.find(id);
	if (it == m_ActiveAudio.end()) return;

	HANDLE_FMOD_ERROR(it->second.pChannel->stop());

	//Note: no need to delete the active audio instance, FMOD callback is called on stop.
}

void rujin::FMOD_AudioProvider::SetAudioPaused(const sound_id id, bool isPaused)
{
	const auto it = m_ActiveAudio.find(id);
	if (it == m_ActiveAudio.end()) return;

	HANDLE_FMOD_ERROR(it->second.pChannel->setPaused(isPaused));
}

bool rujin::FMOD_AudioProvider::IsFinished(const sound_id id)
{
	const auto it = m_ActiveAudio.find(id);
	return it == m_ActiveAudio.end();
}

bool rujin::FMOD_AudioProvider::IsPaused(const sound_id id)
{
	const auto it = m_ActiveAudio.find(id);
	if (it == m_ActiveAudio.end()) return false;

	bool rv;
	HANDLE_FMOD_ERROR(it->second.pChannel->getPaused(&rv));

	return rv;
}

void rujin::FMOD_AudioProvider::LoadAudio(const std::string& filepath)
{
	//check if we already have the sound loaded
	const auto it = m_Sounds.find(filepath); //O(1) 
	if (it != m_Sounds.end())
		return; //we have already loaded this sound.

	//load in the sound
	FMOD::Sound* sound;
	HANDLE_FMOD_ERROR(m_pCore->createSound((ServiceLocator::GetService<ResourceService>().GetDataPath() + filepath).c_str(), FMOD_DEFAULT, nullptr, &sound));

	m_Sounds.insert_or_assign(filepath, sound);
}

void rujin::FMOD_AudioProvider::UnloadAudio(const std::string& filepath)
{
	const auto it = m_Sounds.find(filepath); //O(1)

	if (it == m_Sounds.end())
		return; //return if we didn't find the sound

	//we found the sound, free it and then remove from map.
	HANDLE_FMOD_ERROR(it->second->release());
	m_Sounds.erase(filepath);
}

void rujin::FMOD_AudioProvider::SetMusicVolume(float volume)
{
	m_pMusicChannel->setVolume(volume);
}

void rujin::FMOD_AudioProvider::SetSoundEffectVolume(float volume)
{
	m_pSoundEffectChannel->setVolume(volume);
}

float rujin::FMOD_AudioProvider::GetMusicVolume() const
{
	float rv{};
	HANDLE_FMOD_ERROR(m_pMusicChannel->getVolume(&rv));
	return rv;
}

float rujin::FMOD_AudioProvider::GetSoundEffectVolume() const
{
	float rv{};
	HANDLE_FMOD_ERROR(m_pSoundEffectChannel->getVolume(&rv));
	return rv;
}

rujin::sound_id rujin::FMOD_AudioProvider::PlayAudio(const std::string& filepath, AudioChannel channel)
{
	//find the sound
	FMOD::Sound* sound{ nullptr };
	auto it = m_Sounds.find(filepath); //O(1) 

	//if we do not have the sound, load in the sound first.
	if (it == m_Sounds.end())
	{
		//Warn the user that this is the case, it's best to load the sounds on initialization.
		LOG_WARNING_("Trying to play sound '{}' while it has not been loaded. Automatically loading sound... (Could drastically reduce performance)", filepath);
		
		//load in sound first.
		LoadAudio(filepath);

		it = m_Sounds.find(filepath);

		if (it == m_Sounds.end())
			LOG_ERROR_("Failed to play sound: '{}'. Could not find, or load in sound.");
	}

	sound = it->second;

	FMOD::ChannelGroup* pGroup = nullptr;
	switch (channel)
	{
	default:
	case AudioChannel::SoundEffects:
		pGroup = m_pSoundEffectChannel;
		break;
	case AudioChannel::Music:
		pGroup = m_pMusicChannel;
		break;
	}

	//play sound
	FMOD::Channel* pChannel;
	m_pCore->playSound(sound, pGroup, false, &pChannel);

	sound_id soundId;
	//get unique id for sound.
	if(!m_SoundIdQueue.empty())
	{
		//we have a free id.
		soundId = m_SoundIdQueue.top();
		m_SoundIdQueue.pop();
	}
	else
	{
		//we need to generate and add the next id.
		soundId = m_NextSoundId++;
	}

	//add channel to active audio with an unique_id
	auto* userdata = new ON_FMOD_CHANNELCONTROL_CALLBACK_END_USERDATA{ this, soundId }; //will be de-allocated after callback.
	m_ActiveAudio.insert_or_assign(userdata->sound_id, ActiveAudio{ pChannel, userdata });

	//register callback and userdata for event
	pChannel->setUserData(userdata);
	pChannel->setCallback(&ON_FMOD_CHANNELCONTROL_CALLBACK_END);

	return userdata->sound_id;
}

FMOD_RESULT F_CALL rujin::ON_FMOD_CHANNELCONTROL_CALLBACK_END(FMOD_CHANNELCONTROL* channelcontrol, FMOD_CHANNELCONTROL_TYPE controltype, FMOD_CHANNELCONTROL_CALLBACK_TYPE callbacktype, void* /*param1*/, void* /*param2*/)
{
	if (controltype == FMOD_CHANNELCONTROL_CHANNEL && callbacktype == FMOD_CHANNELCONTROL_CALLBACK_END)
	{
		auto* pChannel = reinterpret_cast<FMOD::Channel*>(channelcontrol);

		ON_FMOD_CHANNELCONTROL_CALLBACK_END_USERDATA* userdata;

		void* pRaw;
		HANDLE_FMOD_ERROR(pChannel->getUserData(&pRaw));
		userdata = static_cast<ON_FMOD_CHANNELCONTROL_CALLBACK_END_USERDATA*>(pRaw);
		ASSERT_NULL(userdata);

		// -- HANDLE SOUND ENDED --
#pragma warning(suppress : 6011) //We assert non-nullptr.
		const sound_id soundId = userdata->sound_id;
		userdata->pAudioProvider->m_ActiveAudio.erase(soundId); // erase from active audio
		userdata->pAudioProvider->m_SoundIdQueue.push(soundId); // push the sound_id back in the queue, so we can reuse it.

		delete userdata;
	}

	return FMOD_OK;
}
