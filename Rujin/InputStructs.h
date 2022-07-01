#pragma once
#include <memory>
#include "Commands.h"

namespace rujin
{
	//Note: 16bit int, to have the 0-256 range, but also -1, can optimize and use the remaining bits in future
	using KeyboardAndMouseButton = int16_t; //VK_... code 
	using PlayerIndex = unsigned char;
	using GamepadIndex = unsigned char;

	enum class ButtonState
	{
		Pressed,
		Released,
		Down,
		Up
	};

	enum class GamepadButton : int32_t
	{
		A = 0x1000,
		B = 0x2000,
		X = 0x4000,
		Y = 0x8000,
		DPAD_UP = 0x0001,
		DPAD_DOWN = 0x0002,
		DPAD_LEFT = 0x0004,
		DPAD_RIGHT = 0x0008,
		START = 0x0010,
		BACK = 0x0020,
		LEFT_STICK = 0x0040,
		RIGHT_STICK = 0x0080,
		LEFT_SHOULDER = 0x0100,
		RIGHT_SHOULDER = 0x0200,
		None = -1
	};

	enum class MouseAxis : int8_t
	{
		X,
		Y,
		None = -1
	};

	//We can always expand this with future supported input devices without breaking old game code!
	struct AxisActionKeybinds
	{
		AxisActionKeybinds(KeyboardAndMouseButton kbOrMouseButton = -1, GamepadButton gpButton = GamepadButton::None, MouseAxis mouseAxis = MouseAxis::None)
			: kbOrMouseButton{ kbOrMouseButton }
			, gpButton{ gpButton }
			, mouseAxis{mouseAxis}
		{}

		KeyboardAndMouseButton kbOrMouseButton = -1;
		GamepadButton gpButton = GamepadButton::None;
		MouseAxis mouseAxis = MouseAxis::None;
	};

	//We can always expand this with future supported input devices without breaking old game code!
	struct InputActionKeybinds
	{
		InputActionKeybinds(ButtonState triggerState = ButtonState::Pressed, KeyboardAndMouseButton kbOrMouseButton = -1, GamepadButton gpButton = GamepadButton::None)
			: triggerState{ triggerState }
			, kbOrMouseButton{ kbOrMouseButton }
			, gpButton{ gpButton }
		{}

		ButtonState triggerState = ButtonState::Pressed;
		KeyboardAndMouseButton kbOrMouseButton = -1;
		GamepadButton gpButton = GamepadButton::None;
	};


	struct GamepadAvailability
	{
		bool isConnected = false;
		bool isInUse = false;
	};
}