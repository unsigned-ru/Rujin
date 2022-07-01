#include "RujinPCH.h"
#include "PlayerInput.h"

rujin::PlayerInput::PlayerInput(PlayerIndex playerIndex)
	: m_PlayerIndex{ playerIndex }
{
}

void rujin::PlayerInput::Update()
{
	
	switch (m_pInputSession->GetInputDeviceType())
	{
		//check what input device we have
		case InputDeviceType::KeyboardAndMouse:
		{
			//check if keyboard keybind is triggered 
			KeyboardAndMouseInputSession* pSession = static_cast<KeyboardAndMouseInputSession*>(m_pInputSession);
			for (auto& pair : m_InputActions)
			{
				auto& button = pair.second.first.kbOrMouseButton;
				auto& isTriggered = pair.second.second;
				const auto& triggerState = pair.second.first.triggerState;

				switch (triggerState)
				{
				case ButtonState::Down:
					isTriggered = pSession->IsKeyboardKeyDown(button);
					break;
				case ButtonState::Up:
					isTriggered = !pSession->IsKeyboardKeyDown(button);
					break;
				case ButtonState::Pressed:
					isTriggered = pSession->IsKeyboardKeyDown(button) && !pSession->IsKeyboardKeyDown(button, true);
					break;
				case ButtonState::Released:
					isTriggered = !pSession->IsKeyboardKeyDown(button) && pSession->IsKeyboardKeyDown(button, true);
					break;
				}
			}

			//for (auto& pair : m_AxisActions)
			//{
			//	//TODO: implement
			//}
			break;
		}
		case InputDeviceType::Gamepad:
		{
			//check if gamepad keybind is triggered 
			GamepadInputSession* pSession = static_cast<GamepadInputSession*>(m_pInputSession);
			
			for (auto& pair : m_InputActions)
			{
				auto& button = pair.second.first.gpButton;
				auto& isTriggered = pair.second.second;
				const auto& triggerState = pair.second.first.triggerState;

				switch (triggerState)
				{
				case ButtonState::Down:
					isTriggered = pSession->IsGamepadButtonDown(button);
					break;
				case ButtonState::Up:
					isTriggered = !pSession->IsGamepadButtonDown(button);
					break;
				case ButtonState::Pressed:
					isTriggered = pSession->IsGamepadButtonDown(button) && !pSession->IsGamepadButtonDown(button, true);
					break;
				case ButtonState::Released:
					isTriggered = !pSession->IsGamepadButtonDown(button) && pSession->IsGamepadButtonDown(button, true);
					break;
				}
			}

			//for (auto& pair : m_AxisActions)
			//{
			//	//TODO: implement
			//}

			break;
		}
	}
}

bool rujin::PlayerInput::IsInputActionTriggered(uint32_t inputAction)
{
	auto it = m_InputActions.find(inputAction);
	
	//check if we found the input action
	if (it != m_InputActions.end())
		//return whether it was triggered
		return it->second.second;

	//we didn't find the input action. Warning!
	LOG_WARNING_("IsInputActionTriggered: non-registered input action detected. ({})", inputAction);
	return false;
}

bool rujin::PlayerInput::IsAxisActionTriggered(uint32_t axisAction)
{
	auto it = m_AxisActions.find(axisAction);

	//check if we found the input action
	if (it != m_AxisActions.end())
		//return whether it was triggered
		return it->second.second;

	//we didn't find the input action. Warning!
	LOG_WARNING_("IsAxisActionTriggered: non-registered axis action detected. ({})", axisAction);
	return false;
}

void rujin::PlayerInput::AddInputAction(uint32_t inputAction, const InputActionKeybinds& keybinds)
{
	m_InputActions.insert_or_assign(inputAction, std::make_pair(keybinds, false));
}

void rujin::PlayerInput::AddAxisAction(uint32_t axisAction, const AxisActionKeybinds& keybinds)
{
	m_AxisActions.insert_or_assign(axisAction, std::make_pair(keybinds, false));
}

void rujin::PlayerInput::RemoveInputAction(uint32_t inputAction)
{
	m_InputActions.erase(inputAction);
}

void rujin::PlayerInput::RemoveAxisAction(uint32_t axisAction)
{
	m_AxisActions.erase(axisAction);
}

bool rujin::PlayerInput::HasInputDevice() const
{
	return m_pInputSession != nullptr;
}

void rujin::PlayerInput::RegisterInputSession(InputSession* pInputSession)
{
	m_pInputSession = pInputSession;
}
