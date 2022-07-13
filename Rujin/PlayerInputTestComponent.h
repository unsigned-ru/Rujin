#pragma once
#include "Component.h"
#include "InputStructs.h"
#include "AudioService.h"
#include "Rutils/DeltaTimer.h"

namespace rujin
{
	class PlayerInputTestComponent final : public Component
	{
	public:
		explicit PlayerInputTestComponent(PlayerIndex playerIndex);

		void Start() override;
		void Update() override;

	private:
		PlayerIndex m_PlayerIndex;

		sound_id m_SoundId = -1;
		bool m_SDL = false;

		float m_Volume = 100;
		float m_VolumeDialSpeed = 1.f;
		rutils::DeltaTimer m_VibrationTimer = rutils::DeltaTimer(2.f, false, false);
	};
}

