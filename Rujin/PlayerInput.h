#pragma once
#include "InputStructs.h"
#include "InputSession.h"

namespace rujin
{
	class PlayerInput final
	{
	public:
		PlayerInput(PlayerIndex playerIndex);

		void Update();

		bool IsInputActionTriggered(uint32_t inputAction);
		bool IsAxisActionTriggered(uint32_t axisAction);

		void AddInputAction(uint32_t inputAction, const InputActionKeybinds& keybinds);
		void AddAxisAction(uint32_t axisAction, const AxisActionKeybinds& keybinds);
		void RemoveInputAction(uint32_t inputAction);
		void RemoveAxisAction(uint32_t axisAction);

		bool HasInputDevice() const;
		void RegisterInputSession(InputSession* pInputSession);

	private:
		PlayerIndex m_PlayerIndex;
		InputSession* m_pInputSession = nullptr;

		std::unordered_map<uint32_t, std::pair<InputActionKeybinds, bool>> m_InputActions{}; //IsTriggeredThisFrame value of pair
		std::unordered_map<uint32_t, std::pair<AxisActionKeybinds, bool>> m_AxisActions{}; //IsTriggeredThisFrame value of pair
	};
}