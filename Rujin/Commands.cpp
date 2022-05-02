#include "RujinPCH.h"
#include "Commands.h"

#include "BurgerComponent.h"
#include "PeterPepperComponent.h"
#include "EnemyComponent.h"
#include "FMOD_AudioProvider.h"
#include "ServiceLocator.h"

rujin::command::Die::Die(PeterPepperComponent* pComp) : IBase(pComp) {};
void rujin::command::Die::Execute()
{
	GetComponent<PeterPepperComponent>()->Die();
}

rujin::command::DropBurger::DropBurger(PeterPepperComponent* pComp, BurgerComponent* pBurger)
	: IBase(pComp)
	, m_pBurger(pBurger)
{}
void rujin::command::DropBurger::Execute()
{
	m_pBurger->Drop(GetComponent<PeterPepperComponent>());
}

rujin::command::KillEnemy::KillEnemy(PeterPepperComponent* pComp, EnemyComponent* pEnemy)
	: IBase(pComp)
	, m_pEnemy(pEnemy)
{
}
void rujin::command::KillEnemy::Execute()
{
	m_pEnemy->Die(GetComponent<PeterPepperComponent>());
}

rujin::command::StopSound::StopSound() : IBase(nullptr) {}

void rujin::command::StopSound::Execute()
{
	ServiceLocator::GetService<AudioService>().StopAudio(m_soundId);
}

rujin::command::ToggleSoundPaused::ToggleSoundPaused() : IBase(nullptr) {}

void rujin::command::ToggleSoundPaused::Execute()
{
	auto& audioService = ServiceLocator::GetService<AudioService>();

	//toggle pause
	audioService.SetAudioPaused(m_soundId, !audioService.IsPaused(m_soundId));
}

rujin::command::PlaySound::PlaySound(const std::string& soundPath, StopSound* pStopCmd, ToggleSoundPaused* pPauseCmd)
	: IBase(nullptr)
	, m_SoundPath(soundPath)
	, m_pStopCmd(pStopCmd)
	, m_pPauseCmd(pPauseCmd)
{}

void rujin::command::PlaySound::Execute()
{
	const auto id = ServiceLocator::GetService<AudioService>().PlaySoundEffect(m_SoundPath);

	if (m_pStopCmd)
		m_pStopCmd->m_soundId = id;

	if (m_pPauseCmd)
		m_pPauseCmd->m_soundId = id;

}

rujin::command::SwitchAudioProvider::SwitchAudioProvider() : IBase(nullptr) {}

void rujin::command::SwitchAudioProvider::Execute()
{
	if (!sdl)
	{
		ServiceLocator::RegisterService<AudioService, SDL_AudioProvider>();
		sdl = true;
		LOG_INFO("ACTIVE AUDIO PROVIDER: SDL_Mixer");
	}
	else
	{
		ServiceLocator::RegisterService<AudioService, FMOD_AudioProvider>();
		sdl = false;
		LOG_INFO("ACTIVE AUDIO PROVIDER: FMOD");
	}
}




