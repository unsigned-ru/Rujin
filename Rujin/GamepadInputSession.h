#ifndef GAMEPAD_INPUT_SESSION_H
#define GAMEPAD_INPUT_SESSION_H


#include "InputSession.h"
#include "InputStructs.h"

namespace rujin
{
	class GamepadInputSession : public InputSession
	{
	public:
		GamepadInputSession() : InputSession(InputDeviceType::Gamepad) {}

		bool UpdateStates() override = 0;

		virtual bool IsGamepadButtonDown(GamepadButton button, bool previousFrame = false) const = 0;

		virtual glm::vec2 GetLeftThumbstickPosition(bool previousFrame = false) const = 0;
		virtual glm::vec2 GetRightThumbstickPosition(bool previousFrame = false) const = 0;

		virtual float GetLeftTriggerPressure(bool previousFrame = false) const = 0;
		virtual float GetRightTriggerPressure(bool previousFrame = false) const = 0;

		virtual void SetVibration(float vibration) = 0;
		virtual void SetLeftVibration(float leftVibration) = 0;
		virtual void SetRightVibration(float rightVibration) = 0;
	};
}


#endif // Include Guard: GAMEPAD_INPUT_SESSION_H
