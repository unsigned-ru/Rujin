#include "RujinPCH.h"
#include "FMOD_AudioProvider.h"

#include "ResourceManager.h"
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
}

rujin::FMOD_AudioProvider::~FMOD_AudioProvider()
{
	if (m_pStudio)
	{
#pragma warning(push)
#pragma warning(disable : 26812)
		//Releases core too.
		m_pStudio->release();

		m_pStudio = nullptr;
		m_pCore = nullptr;
#pragma warning(pop)
	}
}

rujin::sound_id rujin::FMOD_AudioProvider::PlaySound(const std::string& filepath, float /*volume*/)
{
	//find the sound
	FMOD::Sound* sound{nullptr};
	const auto it = m_Sounds.find(filepath); //O(1) 

	//if we do not have the sound, load in the sound first.
	if (it == m_Sounds.end())
	{
		//Warn the user that this is the case, it's best to load the sounds on initialization.
		LOG_WARNING_("Trying to play sound '{}' while it has not been loaded. Automatically loading sound... (Could drastically reduce performance)", filepath);
		//load in sound first.
		LoadSound(filepath);
	}
	else 
		sound = it->second;

	m_pCore->playSound(sound, nullptr, false, nullptr);

	return 0;
}

void rujin::FMOD_AudioProvider::LoadSound(const std::string& filepath)
{
	//check if we already have the sound loaded
	const auto it = m_Sounds.find(filepath); //O(1) 
	if (it != m_Sounds.end())
		return; //we have already loaded this sound.

	//load in the sound
	FMOD::Sound* sound;
	HANDLE_FMOD_ERROR(m_pCore->createSound((ResourceManager::GetInstance().GetDataPath() + filepath).c_str(), FMOD_DEFAULT, nullptr, &sound));

	m_Sounds.insert_or_assign(filepath, sound);
}

void rujin::FMOD_AudioProvider::UnloadSound(const std::string& filepath)
{
	const auto it = m_Sounds.find(filepath); //O(1)

	if (it == m_Sounds.end())
		return; //return if we didn't find the sound

	//we found the sound, free it and then remove from map.
	HANDLE_FMOD_ERROR(it->second->release());
	m_Sounds.erase(filepath);
}
