#include "RujinPCH.h"
#include "InputSession.h"

//TODO: input update frequency

rujin::InputSession::InputSession(const PlayerIndex playerIndex, const InputDeviceType&& deviceType)
	: m_InputDeviceType{deviceType}
	, m_PlayerIndex{ playerIndex }
{
}

rujin::KeyboardAndMouseInputSession::KeyboardAndMouseInputSession(const PlayerIndex playerIndex)
	: InputSession(playerIndex, InputDeviceType::KeyboardAndMouse)
{
	m_pKeyboardState0 = new BYTE[256];
	m_pKeyboardState1 = new BYTE[256];

	m_pCurrKeyboardState = m_pKeyboardState1;
	m_pOldKeyboardState = m_pKeyboardState0;
}

rujin::KeyboardAndMouseInputSession::~KeyboardAndMouseInputSession()
{
	delete[] m_pKeyboardState0;
	delete[] m_pKeyboardState1;

	m_pKeyboardState0 = nullptr;
	m_pKeyboardState1 = nullptr;
	m_pCurrKeyboardState = nullptr;
	m_pOldKeyboardState = nullptr;
}


bool rujin::KeyboardAndMouseInputSession::IsMouseButtonDown(int button, bool previousFrame) const
{
	BYTE* pState = previousFrame ? m_pOldKeyboardState : m_pCurrKeyboardState;

	return (pState[button] & 0xF0) != 0;
}

bool rujin::KeyboardAndMouseInputSession::IsKeyboardKeyDown(int key, bool previousFrame) const
{
	BYTE* pState = previousFrame ? m_pOldKeyboardState : m_pCurrKeyboardState;

	return (pState[key] & 0xF0) != 0;
}

bool rujin::KeyboardAndMouseInputSession::UpdateStates()
{
	if (m_pCurrKeyboardState == m_pKeyboardState0)
	{
		m_pOldKeyboardState = m_pKeyboardState0;
		m_pCurrKeyboardState = m_pKeyboardState1;

		if (!GetKeyboardState(m_pCurrKeyboardState))
			return false; // kb disconnected or something went wrong.
	}
	else
	{
		m_pOldKeyboardState = m_pKeyboardState1;
		m_pCurrKeyboardState = m_pKeyboardState0;

		if (!GetKeyboardState(m_pCurrKeyboardState))
			return false; // kb disconnected or something went wrong.
	}

	//Update mouse position
	m_OldMousePosition = m_CurrMousePosition;
	GetCursorPos(&m_CurrMousePosition);

	m_MouseMovement.x = m_CurrMousePosition.x - m_OldMousePosition.x;
	m_MouseMovement.y = m_CurrMousePosition.y - m_OldMousePosition.y;

	return true;
}

rujin::GamepadInputSession::GamepadInputSession(const PlayerIndex playerIndex, const ControllerIndex controllerIndex, const XINPUT_STATE& state)
	: InputSession(playerIndex, InputDeviceType::Gamepad)
	, m_OldGamepadState{state}
	, m_CurrGamepadState{state}
	, m_ControllerIndex{controllerIndex}
{}

bool rujin::GamepadInputSession::UpdateStates()
{
	m_OldGamepadState = m_CurrGamepadState;
	ZeroMemory(&m_CurrGamepadState, sizeof(XINPUT_STATE));

	// Simply get the state of the controller from XInput.
	DWORD dwResult;
	dwResult = XInputGetState(m_ControllerIndex, &m_CurrGamepadState);

	return !(dwResult != ERROR_SUCCESS);
}

bool rujin::GamepadInputSession::IsGamepadButtonDown(GamepadButton button, bool previousFrame) const
{
	const XINPUT_STATE& state = previousFrame ? m_OldGamepadState : m_CurrGamepadState;

	return state.Gamepad.wButtons & static_cast<uint16_t>(button);
}

glm::vec2 rujin::GamepadInputSession::GetLeftThumbstickPosition(bool previousFrame) const
{
	const XINPUT_STATE& state = previousFrame ? m_OldGamepadState : m_CurrGamepadState;
	glm::vec2 pos{ state.Gamepad.sThumbLX, state.Gamepad.sThumbLY };

	if (pos.x > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && pos.x < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) pos.x = 0;
	if (pos.y > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && pos.y < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) pos.y = 0;

	if (pos.x < 0)
		pos.x /= 32768;
	else
		pos.x /= 32767;

	if (pos.y < 0)
		pos.y /= 32768;
	else 
		pos.y /= 32767;

	return pos;
}

glm::vec2 rujin::GamepadInputSession::GetRightThumbstickPosition(bool previousFrame) const
{
	const XINPUT_STATE& state = previousFrame ? m_OldGamepadState : m_CurrGamepadState;
	glm::vec2 pos{ state.Gamepad.sThumbRX, state.Gamepad.sThumbRY };

	if (pos.x > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && pos.x < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) pos.x = 0;
	if (pos.y > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && pos.y < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) pos.y = 0;

	if (pos.x < 0)
		pos.x /= 32768;
	else
		pos.x /= 32767;

	if (pos.y < 0)
		pos.y /= 32768;
	else
		pos.y /= 32767;

	return pos;
}

float rujin::GamepadInputSession::GetLeftTriggerPressure(bool previousFrame) const
{
	const XINPUT_STATE& state = previousFrame ? m_OldGamepadState : m_CurrGamepadState;

	return state.Gamepad.bLeftTrigger / 255.0f;
}

float rujin::GamepadInputSession::GetRightTriggerPressure(bool previousFrame) const
{
	const XINPUT_STATE& state = previousFrame ? m_OldGamepadState : m_CurrGamepadState;
	return state.Gamepad.bRightTrigger / 255.0f;
}

void rujin::GamepadInputSession::SetVibration(float vibration)
{
	XINPUT_VIBRATION vibrationState;
	vibration = std::clamp(vibration, 0.f, 1.f);

	ZeroMemory(&vibrationState, sizeof(XINPUT_VIBRATION));

	vibrationState.wLeftMotorSpeed = static_cast<WORD>(vibration * 65535);
	vibrationState.wRightMotorSpeed = static_cast<WORD>(vibration * 65535);

	XInputSetState(m_ControllerIndex, &vibrationState);
}

void rujin::GamepadInputSession::SetLeftVibration(float leftVibration)
{
	XINPUT_VIBRATION vibrationState;
	leftVibration = std::clamp(leftVibration, 0.f, 1.f);

	ZeroMemory(&vibrationState, sizeof(XINPUT_VIBRATION));

	vibrationState.wLeftMotorSpeed = static_cast<WORD>(leftVibration * 65535);
	vibrationState.wRightMotorSpeed = 0;

	XInputSetState(m_ControllerIndex, &vibrationState);
}

void rujin::GamepadInputSession::SetRightVibration(float rightVibration)
{
	XINPUT_VIBRATION vibrationState;
	rightVibration = std::clamp(rightVibration, 0.f, 1.f);

	ZeroMemory(&vibrationState, sizeof(XINPUT_VIBRATION));

	vibrationState.wLeftMotorSpeed = 0;
	vibrationState.wRightMotorSpeed = static_cast<WORD>(rightVibration * 65535);;

	XInputSetState(m_ControllerIndex, &vibrationState);
}
