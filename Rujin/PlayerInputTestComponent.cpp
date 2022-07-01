#include "RujinPCH.h"
#include "PlayerInputTestComponent.h"
#include "InputManager.h"
#include "InputActions.h"
#include "ServiceLocator.h"
#include "FMOD_AudioProvider.h"

using namespace rujin;
using namespace rujin::input;

rujin::PlayerInputTestComponent::PlayerInputTestComponent(PlayerIndex playerIndex)
	: m_PlayerIndex{playerIndex}
{
}

void PlayerInputTestComponent::Start()
{
	Component::Start();
}

void PlayerInputTestComponent::Update()
{
	Component::Update();

	const auto& im = input::InputManager::GetInstance();
	auto& ss = ServiceLocator::GetService<AudioService>();

	if (im.IsInputActionTriggered(m_PlayerIndex, (uint32_t)InputAction::StartSound1))
		m_SoundId = ss.PlaySoundEffect("Audio/SFX/rooster_song.wav");

	if (im.IsInputActionTriggered(m_PlayerIndex, (uint32_t)InputAction::StopSound1))
		ss.StopAudio(m_SoundId);

	if (im.IsInputActionTriggered(m_PlayerIndex, (uint32_t)InputAction::PauseSound1))
		ss.SetAudioPaused(m_SoundId, !ss.IsPaused(m_SoundId));

	if (im.IsInputActionTriggered(m_PlayerIndex, (uint32_t)InputAction::SwitchSoundSystem))
	{
		if (!m_SDL)
		{
			ServiceLocator::RegisterService<AudioService, SDL_AudioProvider>();
			m_SDL = true;
			LOG_INFO("ACTIVE AUDIO PROVIDER: SDL_Mixer");
		}
		else
		{
			ServiceLocator::RegisterService<AudioService, FMOD_AudioProvider>();
			m_SDL = false;
			LOG_INFO("ACTIVE AUDIO PROVIDER: FMOD");
		}
	}
	
}