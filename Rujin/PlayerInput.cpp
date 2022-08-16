#include "RujinPCH.h"
#include "PlayerInput.h"

#include "XInputGamepadSession.h"
#include "XInputKeyboardAndMouseSession.h"
#include "Rutils/General.h"

#include <ranges>

rujin::PlayerInput::PlayerInput(const PlayerIndex playerIndex)
	: m_PlayerIndex{ playerIndex }
{
}

rujin::PlayerInput::~PlayerInput()
{
	delete m_pInputSession;
}

void rujin::PlayerInput::Update()
{
	if (!m_pInputSession)
		return;

	//reset all action and axis events
	for (auto& action : m_InputActions | std::views::values)
		action.state.isTriggered = false;

	for (auto& action : m_AxisActions | std::views::values)
	{
		action.state.isTriggered = false;
		action.state.intensity = 0.f;
	}
	
	m_pInputSession->UpdateStates(); //TODO: handle bool (disconnects)

	switch (m_pInputSession->GetInputDeviceType())
	{
		//check what input device we have
		case InputDeviceType::KeyboardAndMouse:
		{
			//Input device is Keyboard and Mouse,
			const auto* pSession = static_cast<XInputKeyboardAndMouseSession*>(m_pInputSession);

			//loop over input action keybinds
			for (auto& [keybinds, actionState] : m_InputActions | std::views::values)
			{
				//check if keyboard keybind is triggered 
				switch (keybinds.triggerState)
				{
				case ButtonState::Down:
					actionState.isTriggered = pSession->IsKeyboardKeyDown(keybinds.kbOrMouseButton);
					break;
				case ButtonState::Up:
					actionState.isTriggered = !pSession->IsKeyboardKeyDown(keybinds.kbOrMouseButton);
					break;
				default:
					LOG_WARNING("Unknown trigger state, defaulting to 'Pressed'");
				case ButtonState::Pressed:
					actionState.isTriggered =
						pSession->IsKeyboardKeyDown(keybinds.kbOrMouseButton) && 
						!pSession->IsKeyboardKeyDown(keybinds.kbOrMouseButton, true);
					break;
				case ButtonState::Released:
					actionState.isTriggered =
						!pSession->IsKeyboardKeyDown(keybinds.kbOrMouseButton) && 
						pSession->IsKeyboardKeyDown(keybinds.kbOrMouseButton, true);
					break;
				}
			}

			//loop over axis action keybinds
			for (auto& [keybinds, actionState] : m_AxisActions | std::views::values)
			{
				// NOTE: Keyboard does not have buttons with intensity.
				// All axis actions bound to keyboard buttons are always -1 | 0 | 1
				if (pSession->IsKeyboardKeyDown(keybinds.kbKeybinds.positive))
					actionState.intensity += 1.f;

				if (pSession->IsKeyboardKeyDown(keybinds.kbKeybinds.negative))
					actionState.intensity -= 1.f;

				if (!rutils::FloatEquals(actionState.intensity, 0))
				{
					actionState.isTriggered = true;
					
					//clamp
					actionState.intensity = std::clamp(actionState.intensity, -1.f, 1.f);
				}

				//Mouse axis (mouse input overrides all other sources of intensity.
				if (keybinds.mouseAxis != MouseAxis::None)
				{
					//TODO: fix mouse sensitivity
					const glm::ivec2 mouseMovement = pSession->GetMouseMovement();

					if (keybinds.mouseAxis == MouseAxis::X && mouseMovement.x != 0)
					{
						actionState.intensity = static_cast<float>(mouseMovement.x);
						actionState.isTriggered = true;
					}
					else if (keybinds.mouseAxis == MouseAxis::Y && mouseMovement.y != 0)
					{
						actionState.intensity = static_cast<float>(mouseMovement.y);
						actionState.isTriggered = true;
					}
				}
			}
			break;
		}
		case InputDeviceType::Gamepad:
		{
			//check if gamepad keybind is triggered 
			const auto* pSession = static_cast<XInputGamepadSession*>(m_pInputSession);
			
			for (auto& [keybinds, actionState] : m_InputActions | std::views::values)
			{
				switch (keybinds.triggerState)
				{
				case ButtonState::Down:
					actionState.isTriggered = pSession->IsGamepadButtonDown(keybinds.gpButton);
					break;
				case ButtonState::Up:
					actionState.isTriggered = !pSession->IsGamepadButtonDown(keybinds.gpButton);
					break;
				default:
					LOG_WARNING("Unknown trigger state, defaulting to 'Pressed'");
				case ButtonState::Pressed:
					actionState.isTriggered = pSession->IsGamepadButtonDown(keybinds.gpButton) && !pSession->IsGamepadButtonDown(keybinds.gpButton, true);
					break;
				case ButtonState::Released:
					actionState.isTriggered = !pSession->IsGamepadButtonDown(keybinds.gpButton) && pSession->IsGamepadButtonDown(keybinds.gpButton, true);
					break;
				}
			}

			//loop over axis action KeybindData and set states.
			for (auto& [keybinds, state] : m_AxisActions | std::views::values)
			{
				//Controller sensitive buttons: [LeftTrigger, RightTrigger, LeftThumbstick, RightThumbstick]

				//Check for positive input.
				switch (keybinds.gpKeybinds.positive)
				{
					// -- sensitive input -- 
				case GamepadButton::LEFT_TRIGGER:
					state.intensity += pSession->GetLeftTriggerPressure();
					break;
				case GamepadButton::RIGHT_TRIGGER:
					state.intensity += pSession->GetRightTriggerPressure();
					break;
				case GamepadButton::LEFT_STICK_X:
					state.intensity += pSession->GetLeftThumbstickPosition().x;
					break;
				case GamepadButton::LEFT_STICK_Y:
					state.intensity += pSession->GetLeftThumbstickPosition().y;
					break;
				case GamepadButton::RIGHT_STICK_X:
					state.intensity += pSession->GetRightThumbstickPosition().x;
					break;
				case GamepadButton::RIGHT_STICK_Y:
					state.intensity += pSession->GetRightThumbstickPosition().y;
					break;
					// -- insensitive input --
				default:
					if (pSession->IsGamepadButtonDown(keybinds.gpKeybinds.positive))
						state.intensity += 1.f;
					break;
				}

				//check for negative input
				switch (keybinds.gpKeybinds.negative)
				{
					// -- sensitive input -- 
				case GamepadButton::LEFT_TRIGGER:
					state.intensity -= pSession->GetLeftTriggerPressure();
					break;
				case GamepadButton::RIGHT_TRIGGER:
					state.intensity -= pSession->GetRightTriggerPressure();
					break;
				case GamepadButton::LEFT_STICK_X:
					state.intensity -= pSession->GetLeftThumbstickPosition().x;
					break;
				case GamepadButton::LEFT_STICK_Y:
					state.intensity -= pSession->GetLeftThumbstickPosition().y;
					break;
				case GamepadButton::RIGHT_STICK_X:
					state.intensity -= pSession->GetRightThumbstickPosition().x;
					break;
				case GamepadButton::RIGHT_STICK_Y:
					state.intensity -= pSession->GetRightThumbstickPosition().y;
					break;
					// -- insensitive input --
				default:
					if (pSession->IsGamepadButtonDown(keybinds.gpKeybinds.negative))
						state.intensity -= 1.f;
					break;
				}

				if (!rutils::FloatEquals(state.intensity, 0))
				{
					state.isTriggered = true;
					state.intensity = std::clamp(state.intensity, -1.f, 1.f);
				}
			}
			break;
		}
		default:
			assert(!"Unsupported Input device.");
			return;
	}
}

bool rujin::PlayerInput::IsInputActionTriggered(uint32_t inputAction) const
{
	//check if we found the input action
	if (const auto it = m_InputActions.find(inputAction); it != m_InputActions.end())
		return it->second.state.isTriggered; //return whether it was triggered

	//we didn't find the input action. Warning!
	LOG_WARNING_("IsInputActionTriggered: non-registered input action detected. ({})", inputAction);
	return false;
}

bool rujin::PlayerInput::IsAxisActionTriggered(uint32_t axisAction, float* pIntensity) const
{
	//check if we found the input action
	if (const auto it = m_AxisActions.find(axisAction); it != m_AxisActions.end())
	{
		if (pIntensity)
			*pIntensity = it->second.state.intensity;
		return it->second.state.isTriggered;
	}

	//we didn't find the input action. Warning!
	LOG_WARNING_("IsAxisActionTriggered: non-registered axis action detected. ({})", axisAction);
	return false;
}

void rujin::PlayerInput::SetLeftVibration(const float intensity)
{
	switch (m_pInputSession->GetInputDeviceType())
	{
	default:
	case InputDeviceType::KeyboardAndMouse: 
		break;
	case InputDeviceType::Gamepad:
		reinterpret_cast<XInputGamepadSession*>(m_pInputSession)->SetLeftVibration(intensity);
		break;
	}
}

void rujin::PlayerInput::SetRightVibration(float intensity)
{
	switch (m_pInputSession->GetInputDeviceType())
	{
	default:
	case InputDeviceType::KeyboardAndMouse:
		break;
	case InputDeviceType::Gamepad:
		reinterpret_cast<XInputGamepadSession*>(m_pInputSession)->SetRightVibration(intensity);
		break;
	}
}

void rujin::PlayerInput::SetVibration(float intensity)
{
	switch (m_pInputSession->GetInputDeviceType())
	{
	default:
	case InputDeviceType::KeyboardAndMouse:
		break;
	case InputDeviceType::Gamepad:
		reinterpret_cast<XInputGamepadSession*>(m_pInputSession)->SetVibration(intensity);
		break;
	}
}

void rujin::PlayerInput::AddInputAction(uint32_t inputAction, const InputActionKeybinds& keybinds)
{
	if (const auto it = m_InputActions.find(inputAction); it != m_InputActions.end())
		//we found a pre-existing input action, remove old one.
		m_InputActions.erase(it);

	m_InputActions.insert(std::make_pair(inputAction, InputActionData{ keybinds, InputActionState() }));
}

void rujin::PlayerInput::AddAxisAction(uint32_t axisAction, const AxisActionKeybinds& keybinds)
{
	if (const auto it = m_AxisActions.find(axisAction); it != m_AxisActions.end())
		//we found a pre-existing axis action, remove old one.
		m_AxisActions.erase(it);

	m_AxisActions.insert(std::make_pair(axisAction, AxisActionData{ keybinds, AxisActionState() }));
}

void rujin::PlayerInput::RemoveInputAction(const uint32_t inputAction)
{
	m_InputActions.erase(inputAction);
}

void rujin::PlayerInput::RemoveAxisAction(const uint32_t axisAction)
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
