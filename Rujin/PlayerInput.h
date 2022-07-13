#pragma once
#include <unordered_map>

#include "InputStructs.h"
#include "InputSession.h"

namespace rujin
{
	class PlayerInput final
	{
	public:
		explicit PlayerInput(PlayerIndex playerIndex);

		void Update();

		bool IsInputActionTriggered(uint32_t inputAction) const;
		bool IsAxisActionTriggered(uint32_t axisAction, float* pIntensity = nullptr) const;

		void SetLeftVibration(float intensity);
		void SetRightVibration(float intensity);
		void SetVibration(float intensity);

		void AddInputAction(uint32_t inputAction, const InputActionKeybinds& keybinds);
		void AddAxisAction(uint32_t axisAction, const AxisActionKeybinds& keybinds);
		void RemoveInputAction(uint32_t inputAction);
		void RemoveAxisAction(uint32_t axisAction);

		bool HasInputDevice() const;
		void RegisterInputSession(InputSession* pInputSession);

	private:
		PlayerIndex m_PlayerIndex;
		InputSession* m_pInputSession = nullptr;

		std::unordered_map<uint32_t, InputActionData> m_InputActions{};
		std::unordered_map<uint32_t, AxisActionData> m_AxisActions{};
	};
}