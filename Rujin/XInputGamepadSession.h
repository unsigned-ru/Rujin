#pragma once
#pragma comment(lib, "xinput.lib")
#include <Xinput.h>
#include <glm/vec2.hpp>

#include "GamePadInputSession.h"
#include "InputStructs.h"

namespace rujin
{
	class XInputGamepadSession final : public GamepadInputSession
	{
	public:
		explicit XInputGamepadSession(const GamepadIndex gamepadIndex, const XINPUT_STATE& state);
		~XInputGamepadSession() override = default;

		XInputGamepadSession(const XInputGamepadSession&) = delete;
		XInputGamepadSession(XInputGamepadSession&&) noexcept = delete;

		XInputGamepadSession operator=(XInputGamepadSession&&) noexcept = delete;
		XInputGamepadSession operator=(const XInputGamepadSession&) = delete;

		bool UpdateStates() override;

		bool IsGamepadButtonDown(GamepadButton button, bool previousFrame = false) const override;

		glm::vec2 GetLeftThumbstickPosition(bool previousFrame = false) const override;
		glm::vec2 GetRightThumbstickPosition(bool previousFrame = false) const override;

		float GetLeftTriggerPressure(bool previousFrame = false) const override;
		float GetRightTriggerPressure(bool previousFrame = false) const override;

		void SetVibration(float vibration) override;
		void SetLeftVibration(float leftVibration) override;
		void SetRightVibration(float rightVibration) override;

		GamepadIndex GetGamepadIndex() const { return m_GamepadIndex; };

	private:
		XINPUT_STATE m_OldGamepadState{};
		XINPUT_STATE m_CurrGamepadState{};

		GamepadIndex m_GamepadIndex;
	};
}
