#pragma once
#include "InputStructs.h"
#include <Xinput.h>

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
		InputSession(const PlayerIndex playerIndex, const InputDeviceType&& deviceType);
		virtual ~InputSession() = default;

		//returns false if input device disconnects.
		virtual bool UpdateStates() = 0;

		PlayerIndex GetPlayerIndex() const { return m_PlayerIndex; };
		InputDeviceType GetInputDeviceType() const { return m_InputDeviceType; };

	private:
		PlayerIndex m_PlayerIndex;
		InputDeviceType m_InputDeviceType;
	};

	template <typename ButtonType>
	class InputSessionCommands
	{
	public:
		InputSessionCommands() = default;
		virtual ~InputSessionCommands() = default;

	protected:
		std::vector<KeybindData<ButtonType>> m_KeybindCommands{};
	};

	class KeyboardAndMouseInputSession final : public InputSession, public InputSessionCommands<KeyboardAndMouseButton>
	{
	public:
		KeyboardAndMouseInputSession(const PlayerIndex playerIndex);
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


	class GamepadInputSession final : public InputSession, public InputSessionCommands<GamepadButton>
	{
	public:
		GamepadInputSession(const PlayerIndex playerIndex, const ControllerIndex controllerIndex, const XINPUT_STATE& state);
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

		ControllerIndex GetGamepadIndex() const { return m_ControllerIndex; };

	private:
		XINPUT_STATE m_OldGamepadState{};
		XINPUT_STATE m_CurrGamepadState{};

		//point to one of the above
		BYTE* m_pCurrKeyboardState{};
		BYTE* m_pOldKeyboardState{};

		POINT m_OldMousePosition{};
		POINT m_CurrMousePosition{};
		POINT m_MouseMovement{};

		ControllerIndex m_ControllerIndex;
	};
}