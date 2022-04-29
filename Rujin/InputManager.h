#pragma once
#include <memory>

#include "Singleton.h"


namespace rujin
{
	namespace command
	{
		class IBase;
	};

	namespace input
	{
		enum class ControllerButton
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

		enum class ButtonState
		{
			Pressed,
			Released,
			Down,
			Up
		};

		struct KeybindData
		{
			KeybindData(const ControllerButton controllerButton, const ButtonState triggerState, std::unique_ptr<command::IBase>& pCommand);
			~KeybindData();

			KeybindData(const KeybindData& other) = delete;
			KeybindData(KeybindData&& other) noexcept;
			KeybindData& operator=(const KeybindData& other) = delete;
			KeybindData& operator=(KeybindData&& other) noexcept;


			//Button that will trigger the command
			ControllerButton controllerButton;

			//required state of the button that will trigger the command
			ButtonState triggerState;

			//command that will be executed when the button requirements are met.
			std::unique_ptr<command::IBase> pCommand;
		};

		class InputManager final : public Singleton<InputManager>
		{
		public:
			InputManager();
			~InputManager() override;

			InputManager(InputManager&) = delete;
			InputManager& operator=(InputManager&) = delete;
			InputManager(InputManager&&) = delete;
			InputManager& operator=(InputManager&&) = delete;

			bool ProcessInput() const;
			bool IsDown(ControllerButton button) const;
			bool IsPressedThisFrame(ControllerButton button) const;
			bool IsReleasedThisFrame(ControllerButton button) const;


			void RegisterCommand(ControllerButton button, std::unique_ptr<command::IBase>&& command, ButtonState triggerState = ButtonState::Released) const;

		private:
			class InputManagerImpl;
			std::unique_ptr<InputManagerImpl> m_pImpl;
		};

	}
}
