#pragma once
#include <memory>

namespace rujin
{
	struct GamepadAvailability
	{
		bool isConnected = false;
		bool isInUse = false;
	};
	
	//Note: 16bit int, to have the 0-256 range, but also -1, can optimize and use the remaining bits in future
	using KeyboardAndMouseButton = int16_t; //VK_... code 
	using PlayerIndex = unsigned char;
	using GamepadIndex = unsigned char;

	// Indiciates the state of a button.
	enum class ButtonState
	{
		Pressed,
		Released,
		Down,
		Up
	};

	// Possible gamepad buttons.
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
		LEFT_STICK_X,
		LEFT_STICK_Y,
		RIGHT_STICK = 0x0080,
		RIGHT_STICK_X,
		RIGHT_STICK_Y,
		LEFT_SHOULDER = 0x0100,
		RIGHT_SHOULDER = 0x0200,
		LEFT_TRIGGER,
		RIGHT_TRIGGER,
		None = -1
	};

	// Mouse axes
	enum class MouseAxis : int8_t
	{
		X,
		Y,
		None = -1
	};

	/// <summary>
	/// <para>Struct that holds Keybinds to a Input Action</para>
	///	<para>Note: Input actions are binary. An action either is triggered by a keybind, or not.</para>
	/// </summary>
	struct InputActionKeybinds final
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

	// Holds the state of an input action.
	struct InputActionState final
	{
		bool isTriggered = false;
	};

	// Holds both keybinds and state of an input action.
	struct InputActionData final
	{
		const InputActionKeybinds keybinds;
		InputActionState state;
	};

	/// <summary>
	/// Struct that holds positive and negative axis information for keyboard.
	/// <para>Note: Sensitivity for keyboard axis events is always: 1 || 0 || -1</para>
	/// </summary>
	struct KeyboardAxisKeybinds final
	{
		explicit KeyboardAxisKeybinds(KeyboardAndMouseButton positive = -1, KeyboardAndMouseButton negative = -1)
			: positive{positive}
			, negative{negative}
		{}

		KeyboardAndMouseButton positive = -1;
		KeyboardAndMouseButton negative = -1;

		static KeyboardAxisKeybinds None() { return KeyboardAxisKeybinds(); }
	};

	/// <summary>
	/// Struct that holds positive and negative axis information for gamepad.
	/// </summary>
	struct GamepadAxisKeybinds final
	{
		explicit GamepadAxisKeybinds(GamepadButton positive = GamepadButton::None, GamepadButton negative = GamepadButton::None)
			: positive{ positive }
			, negative{ negative }
		{}

		GamepadButton positive = GamepadButton::None;
		GamepadButton negative = GamepadButton::None;

		static GamepadAxisKeybinds None() { return GamepadAxisKeybinds(); }
	};

	/// <summary>
	/// <para>Struct that holds Keybinds to a axis Action</para>
	///	<para>Note: Axis actions are linear with a -1 to 1 range.</para>
	/// <para>Exception: Mouse axis actions will fill the sensitivity with the amount of pixels the mouse moved.</para>
	/// </summary>
	struct AxisActionKeybinds final
	{
		AxisActionKeybinds
		(
			KeyboardAxisKeybinds kbKeybinds = KeyboardAxisKeybinds::None()
			, GamepadAxisKeybinds gpKeybinds = GamepadAxisKeybinds::None()
			, MouseAxis mouseAxis = MouseAxis::None
		)
			: kbKeybinds{ kbKeybinds }
			, gpKeybinds{ gpKeybinds }
			, mouseAxis{mouseAxis}
		{}

		KeyboardAxisKeybinds kbKeybinds = KeyboardAxisKeybinds::None();
		GamepadAxisKeybinds gpKeybinds = GamepadAxisKeybinds::None();
		MouseAxis mouseAxis = MouseAxis::None;
	};

	/// <summary>
	/// <para>Struct that holds the axis action state</para>
	///	<para>Note: Sensitivity is linear, -1 to 1 range.</para>
	/// <para>Exception: Mouse axis actions will fill the sensitivity with the amount of pixels the mouse moved.</para>
	/// </summary>
	struct AxisActionState
	{
		bool isTriggered = false;
		float intensity = 0.f;
	};

	// Holds both keybinds and state of an axis action.
	struct AxisActionData final
	{
		const AxisActionKeybinds keybinds;
		AxisActionState state;
	};
}