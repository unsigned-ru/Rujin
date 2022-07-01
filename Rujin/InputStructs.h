#pragma once
#include <memory>
#include "Commands.h"

namespace rujin
{
	

	enum class GamepadButton : uint16_t
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
	};

	
	using KeyboardAndMouseButton = unsigned char; //VK_... code 
	using PlayerIndex = unsigned char;
	using ControllerIndex = unsigned char;

	enum class ButtonState
	{
		Pressed,
		Released,
		Down,
		Up
	};

	template <typename ButtonType>
	struct KeybindData
	{
		KeybindData(const ButtonType button, const ButtonState triggerState, std::unique_ptr<command::IBase>& pCommand)
			: m_Button(button)
			, m_TriggerState(triggerState)
			, m_pCommand(std::move(pCommand))
		{}

		~KeybindData() = default;

		KeybindData(const KeybindData& other) = delete;
		KeybindData(KeybindData&& other) noexcept
			: m_Button(other.m_Button)
			, m_TriggerState(other.m_TriggerState)
			, m_pCommand(std::move(other.m_pCommand))
		{
		}
		KeybindData& operator=(const KeybindData& other) = delete;
		KeybindData& operator=(KeybindData&& other) noexcept
		{
			this->m_Button = other.m_Button;
			this->m_TriggerState = other.m_TriggerState;
			this->m_pCommand = std::move(other.m_pCommand);
		}

		//Button that will trigger the command
		ButtonType m_Button;

		//required state of the button that will trigger the command
		ButtonState m_TriggerState;

		//command that will be executed when the button requirements are met.
		std::unique_ptr<command::IBase> m_pCommand;
	};

}