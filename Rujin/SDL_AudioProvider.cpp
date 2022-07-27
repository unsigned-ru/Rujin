#include "RujinPCH.h"
#include "SDL_AudioProvider.h"
#include <chrono>
#include <functional>
#include <thread>

#include "ResourceProvider.h"
#include "ServiceLocator.h"

rujin::SDL_AudioProvider::SDL_AudioProvider()
	: m_FillQueue{}
	, m_ThreadQueue{}
	, m_pAudioThread(new std::thread(&SDL_AudioProvider::RunThread, this))
	, m_ExitThread(false)
{
	//Initialize SDL_mixer
	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
	{
		LOG_ERROR(std::string("SDL_Init mixer Error: ") + Mix_GetError());
	}
}

rujin::SDL_AudioProvider::~SDL_AudioProvider()
{
	//set the thread exit flag
	m_ExitThread = true;

	//join if the thread is still at work
	if (m_pAudioThread->joinable())
		m_pAudioThread->join();

	//thread finished work...
	//delete the thread
	delete m_pAudioThread;

	//free chunks
	for (auto& pair : m_Audio)
		Mix_FreeChunk(pair.second);

	//Quit SDL_mixer
	Mix_CloseAudio();
}

rujin::sound_id rujin::SDL_AudioProvider::PlaySoundEffect(const std::string& filepath)
{
	std::lock_guard<std::mutex> lock(m_QueueMutex);
	m_FillQueue.emplace_back(filepath);
	return 0;
}

void rujin::SDL_AudioProvider::LoadAudio(const std::string& filepath)
{
	//check if we have already Loaded audio
	const auto it = m_Audio.find(filepath);

	if (it != m_Audio.end())
		return; //the audio is already loaded

	//we haven't loaded the audio yet, load the audio with SDL
	Mix_Chunk* pAudio = Mix_LoadWAV((ServiceLocator::GetService<ResourceService>().GetDataPath() + filepath).c_str());

	if (!pAudio)
		LOG_ERROR_("Failed to load audio file: \"{}\" (only .wav files are supported)");

	//successfully loaded audio.
	m_Audio.insert_or_assign(filepath, pAudio);
}

void rujin::SDL_AudioProvider::UnloadAudio(const std::string& filepath)
{
	//find audio to unload
	const auto it = m_Audio.find(filepath);

	if (it == m_Audio.end())
		return; //audio is not loaded.

	//unload Audio via SDL.
	Mix_FreeChunk(it->second);

	//erase from dict
	m_Audio.erase(it);
}

void rujin::SDL_AudioProvider::RunThread()
{
	/* Initialization */

	/* update loop */
	while (!m_ExitThread)
	{
		auto start = std::chrono::high_resolution_clock::now();
	
		if (!m_FillQueue.empty())
		{
			//swap fill and thread buffer, clear new fill buffer for new soundqueue's
			SwapQueueBuffers();

			//play queued' sounds
			for (const AudioRequest& req : m_ThreadQueue)
			{
				//find requested loaded file
				const auto it = m_Audio.find(req.filepath);

				if (it == m_Audio.end())
				{
					//we do not have the audio loaded in
					//warn the user about loading audio post-initialization
					//load in the audio
					LOG_WARNING_("Loading in \"{}\" post-initialization. Can cause huge performance drops, concider loading in the file during initialization", req.filepath);
					LoadAudio(req.filepath);
				}

				Mix_Chunk* pChunk = m_Audio.at(req.filepath);

				//play sound
				Mix_PlayChannel(-1, pChunk, 0);
			}
		}

		auto end = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

		if (duration.count() < C_POLLING_RATE)
			std::this_thread::sleep_for(duration); //sleep for the remaining duration
	}
}

void rujin::SDL_AudioProvider::SwapQueueBuffers()
{
	//swap buffers
	std::lock_guard<std::mutex> lock(m_QueueMutex);
	std::swap(m_FillQueue, m_ThreadQueue);

	//reset fill queue
	m_FillQueue.clear();
}
