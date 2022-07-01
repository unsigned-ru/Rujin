#pragma once
#include "Component.h"
#include "InputStructs.h"
#include "AudioService.h"

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
	};
}

