#include "RujinPCH.h"
#include "SDL_AudioProvider.h"
#include <chrono>
#include <thread>

rujin::services::SDL_AudioProvider::SDL_AudioProvider()
	: m_pAudioThread(new std::thread(&SDL_AudioProvider::Update, this))
	, m_ExitThread(false)
{
	m_pAudioThread->detach();
}

rujin::services::SDL_AudioProvider::~SDL_AudioProvider()
{
	//set the thread exit flag
	m_ExitThread = true;

	//join if the thread is still at work
	if (m_pAudioThread->joinable())
		m_pAudioThread->join();

	//thread finished work...
	//delete the thread
	delete m_pAudioThread;
}

void rujin::services::SDL_AudioProvider::PlaySound(const uint16_t /*id*/, const float /*volume*/)
{
}

void rujin::services::SDL_AudioProvider::StopSound(const uint16_t /*id*/)
{
}

void rujin::services::SDL_AudioProvider::StopAllSounds()
{
}

void rujin::services::SDL_AudioProvider::Update()
{
	while (!m_ExitThread)
	{
		auto start = std::chrono::high_resolution_clock::now();

		// do some audio stuff here

		std::cout << "Running..." << std::endl;

		auto end = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

		if (duration.count() < C_POLLING_RATE)
			std::this_thread::sleep_for(duration); //sleep for the remaining duration
	}
}
