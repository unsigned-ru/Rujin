#pragma once
#include "InputStructs.h"
#include <Xinput.h>
#include <unordered_map>

namespace rujin
{
	enum class InputDeviceType : int
	{
		KeyboardAndMouse,
		Gamepad
	};

	class InputSession
	{
	public:
		InputSession(const InputDeviceType&& deviceType);
		virtual ~InputSession() = default;

		//returns false if input device disconnects.
		virtual bool UpdateStates() = 0;

		InputDeviceType GetInputDeviceType() const { return m_InputDeviceType; };

	private:
		InputDeviceType m_InputDeviceType;
	};

	class KeyboardAndMouseInputSession final : public InputSession
	{
	public:
		KeyboardAndMouseInputSession();
		~KeyboardAndMouseInputSession();

		bool IsMouseButtonDown(int button, bool previousFrame = false) const;
		bool IsKeyboardKeyDown(int key, bool previousFrame = false) const;

		virtual bool UpdateStates() override;

	private:
		BYTE* m_pKeyboardState0{};
		BYTE* m_pKeyboardState1{};

		//point to one of the above
		BYTE* m_pCurrKeyboardState{};
		BYTE* m_pOldKeyboardState{};

		POINT m_OldMousePosition{};
		POINT m_CurrMousePosition{};
		POINT m_MouseMovement{};
	};


	class GamepadInputSession final : public InputSession
	{
	public:
		GamepadInputSession(const GamepadIndex gamepadIndex, const XINPUT_STATE& state);
		~GamepadInputSession() = default;

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