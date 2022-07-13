#pragma once
#include <Xinput.h>
#include <glm/vec2.hpp>

#include "InputSession.h"
#include "InputStructs.h"

namespace rujin
{
	class GamepadInputSession final : public InputSession
	{
	public:
		explicit GamepadInputSession(const GamepadIndex gamepadIndex, const XINPUT_STATE& state);
		~GamepadInputSession() override = default;

		GamepadInputSession(const GamepadInputSession&) = delete;
		GamepadInputSession(GamepadInputSession&&) noexcept = delete;

		GamepadInputSession operator=(GamepadInputSession&&) noexcept = delete;
		GamepadInputSession operator=(const GamepadInputSession&) = delete;
		

		virtual bool UpdateStates() override;

		bool IsGamepadButtonDown(GamepadButton button, bool previousFrame = false) const;

		glm::vec2 GetLeftThumbstickPosition(bool previousFrame = false) const;
		glm::vec2 GetRightThumbstickPosition(bool previousFrame = false) const;

		float GetLeftTriggerPressure(bool previousFrame = false) const;
		float GetRightTriggerPressure(bool previousFrame = false) const;

		void SetVibration(float vibration);
		void SetLeftVibration(float leftVibration);
		void SetRightVibration(float rightVibration);

		GamepadIndex GetGamepadIndex() const { return m_GamepadIndex; };

	private:
		XINPUT_STATE m_OldGamepadState{};
		XINPUT_STATE m_CurrGamepadState{};

		GamepadIndex m_GamepadIndex;
	};
}
