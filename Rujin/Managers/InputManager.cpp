#include "RujinPCH.h"
#include "InputManager.h"

#include "Commands.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Xinput.h>
#pragma comment(lib, "xinput.lib")
#include <vector>



namespace rujin::input
{
	class InputManager::InputManagerImpl final
	{
	public:
		bool ProcessInput()
		{
			CopyMemory(&m_PreviousState, &m_CurrentState, sizeof(XINPUT_STATE));
			ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
			XInputGetState(0, &m_CurrentState);

			auto buttonChanges = m_CurrentState.Gamepad.wButtons ^ m_PreviousState.Gamepad.wButtons;
			m_ButtonsPressedThisFrame = buttonChanges & m_CurrentState.Gamepad.wButtons;
			m_ButtonsReleasedThisFrame = buttonChanges & (~m_CurrentState.Gamepad.wButtons);


			//check for registered commands
			for (const KeybindData& keybind : m_KeybindCommands)
			{
				switch (keybind.triggerState)
				{
				case ButtonState::Pressed:
					if (IsPressedThisFrame(keybind.controllerButton))
						keybind.pCommand->Execute();
					break;
				case ButtonState::Released:
					if (IsReleasedThisFrame(keybind.controllerButton))
						keybind.pCommand->Execute();
					break;
				case ButtonState::Down:
					if (IsDown(keybind.controllerButton))
						keybind.pCommand->Execute();
					break;
				case ButtonState::Up:
					if (!IsDown(keybind.controllerButton))
						keybind.pCommand->Execute();
					break;
				}
			}

			//TODO: maybe some exit condition that will return false
			return true;
		}
		bool IsDown(ControllerButton button) const
		{
			return m_CurrentState.Gamepad.wButtons & static_cast<WORD>(button);
		}
		bool IsPressedThisFrame(ControllerButton button) const
		{
			return m_ButtonsPressedThisFrame & static_cast<WORD>(button);
		}
		bool IsReleasedThisFrame(ControllerButton button) const
		{
			return m_ButtonsReleasedThisFrame & static_cast<WORD>(button);
		}
		void RegisterCommand(ControllerButton button, std::unique_ptr<command::Base>&& command, ButtonState triggerState = ButtonState::Released)
		{
			m_KeybindCommands.push_back({ button, triggerState, command });
		}

	private:
		std::vector<KeybindData> m_KeybindCommands{};

		XINPUT_STATE m_CurrentState{};
		XINPUT_STATE m_PreviousState{};
		WORD m_ButtonsPressedThisFrame{};
		WORD m_ButtonsReleasedThisFrame{};
	};

	//ctor and dtor -- NEED to be declared AFTER the implementation class definition in .cpp
	InputManager::InputManager() : m_pImpl(std::make_unique<InputManagerImpl>()) {}
	InputManager::~InputManager() = default;


	KeybindData::KeybindData(const ControllerButton controllerButton, const ButtonState triggerState, std::unique_ptr<command::Base>& pCommand)
		: controllerButton(controllerButton)
		, triggerState(triggerState)
		, pCommand(std::move(pCommand))
	{}
	KeybindData::~KeybindData() = default;
	KeybindData::KeybindData(KeybindData && other) noexcept
		: controllerButton(other.controllerButton)
		, triggerState(other.triggerState)
		, pCommand(std::move(other.pCommand))
	{
	}
	KeybindData& KeybindData::operator=(KeybindData && other) noexcept
	{
		controllerButton = other.controllerButton;
		triggerState = other.triggerState;
		pCommand = std::move(other.pCommand);

		return *this;
	}

#pragma region pImpl_Propogation
	bool InputManager::ProcessInput() const
	{
		return m_pImpl->ProcessInput();
	}
	bool InputManager::IsDown(ControllerButton button) const
	{
		return m_pImpl->IsDown(button);
	}
	bool InputManager::IsPressedThisFrame(ControllerButton button) const
	{
		return m_pImpl->IsPressedThisFrame(button);
	}
	bool InputManager::IsReleasedThisFrame(ControllerButton button) const
	{
		return m_pImpl->IsReleasedThisFrame(button);
	}
	void InputManager::RegisterCommand(ControllerButton button, std::unique_ptr<command::Base>&& pCommand, ButtonState triggerState) const
	{
		m_pImpl->RegisterCommand(button, std::move(pCommand), triggerState);
	}
#pragma endregion
}