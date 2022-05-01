#include "RujinPCH.h"
#include "SDL_AudioProvider.h"
#include <chrono>
#include <functional>
#include <thread>

#include "ServiceLocator.h"

rujin::SDL_AudioProvider::SDL_AudioProvider()
	: m_pAudioThread(new std::thread(&SDL_AudioProvider::RunThread, this))
	, m_ExitThread(false)
{
	//Initialize SDL_mixer
	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
	{
		LOG_ERROR(std::string("SDL_Init mixer Error: ") + Mix_GetError());
	}

	m_pAudioThread->detach();
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

	Mix_ChannelFinished(nullptr);

	//Quit SDL_mixer
	Mix_CloseAudio();
}

rujin::sound_id rujin::SDL_AudioProvider::PlaySound(const std::string& /*clip*/, float /*volume*/)
{
	//add to the queue
	std::lock_guard guard(m_QueueMutex);
	//m_AudioQueue.push_back({ clip, volume });


	return 0;
}

void rujin::SDL_AudioProvider::LoadSound(const std::string& /*filepath*/)
{
}

void rujin::SDL_AudioProvider::UnloadSound(const std::string& /*filepath*/)
{
}

void rujin::SDL_AudioProvider::RunThread()
{
	///* Initialization */

	///* update loop */
	//while (!m_ExitThread)
	//{
	//	auto start = std::chrono::high_resolution_clock::now();

	//	m_QueueMutex.lock();
	//	auto queue = m_AudioQueue;
	//	m_AudioQueue.clear();
	//	m_QueueMutex.unlock();

	//	//play queued' sounds
	//	for (const AudioRequest& req : queue)
	//	{
	//		Mix_PlayChannel(-1, req.clip->GetChunk(), 0);
	//	}


	//	auto end = std::chrono::high_resolution_clock::now();
	//	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

	//	if (duration.count() < C_POLLING_RATE)
	//		std::this_thread::sleep_for(duration); //sleep for the remaining duration
	//}
}
