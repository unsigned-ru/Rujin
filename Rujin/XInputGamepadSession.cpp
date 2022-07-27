#include "RujinPCH.h"
#include "XInputGamepadSession.h"

#include "Rutils/General.h"

rujin::XInputGamepadSession::XInputGamepadSession(const GamepadIndex gamepadIndex, const XINPUT_STATE& state)
	: GamepadInputSession()
	, m_OldGamepadState{ state }
	, m_CurrGamepadState{ state }
	, m_GamepadIndex{ gamepadIndex }
{}

bool rujin::XInputGamepadSession::UpdateStates()
{
	m_OldGamepadState = m_CurrGamepadState;
	ZeroMemory(&m_CurrGamepadState, sizeof(XINPUT_STATE));

	const DWORD dwResult = XInputGetState(m_GamepadIndex, &m_CurrGamepadState);

	return !(dwResult != ERROR_SUCCESS);
}

bool rujin::XInputGamepadSession::IsGamepadButtonDown(GamepadButton button, bool previousFrame) const
{
	if (button == GamepadButton::None)
		return false;

	const XINPUT_STATE& state = previousFrame ? m_OldGamepadState : m_CurrGamepadState;

	switch (button)
	{
	case GamepadButton::LEFT_TRIGGER:
		return !rutils::FloatEquals(GetLeftTriggerPressure(previousFrame), 0);
	case GamepadButton::RIGHT_TRIGGER:
		return !rutils::FloatEquals(GetRightTriggerPressure(previousFrame), 0);
	case GamepadButton::LEFT_STICK_X:
		return !rutils::FloatEquals(GetLeftThumbstickPosition(previousFrame).x, 0);
	case GamepadButton::LEFT_STICK_Y:
		return !rutils::FloatEquals(GetLeftThumbstickPosition(previousFrame).y, 0);
	case GamepadButton::RIGHT_STICK_X:
		return !rutils::FloatEquals(GetRightThumbstickPosition(previousFrame).x, 0);
	case GamepadButton::RIGHT_STICK_Y:
		return !rutils::FloatEquals(GetRightThumbstickPosition(previousFrame).y, 0);
	default:
		return state.Gamepad.wButtons & static_cast<uint16_t>(button);
	}
}

glm::vec2 rujin::XInputGamepadSession::GetLeftThumbstickPosition(bool previousFrame) const
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

glm::vec2 rujin::XInputGamepadSession::GetRightThumbstickPosition(bool previousFrame) const
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

float rujin::XInputGamepadSession::GetLeftTriggerPressure(bool previousFrame) const
{
	const XINPUT_STATE& state = previousFrame ? m_OldGamepadState : m_CurrGamepadState;

	if (state.Gamepad.bLeftTrigger < XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
		return 0;

	return static_cast<float>(state.Gamepad.bLeftTrigger) / 255.0f;
}

float rujin::XInputGamepadSession::GetRightTriggerPressure(bool previousFrame) const
{
	const XINPUT_STATE& state = previousFrame ? m_OldGamepadState : m_CurrGamepadState;

	if (state.Gamepad.bRightTrigger < XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
		return 0;

	return static_cast<float>(state.Gamepad.bRightTrigger) / 255.0f;
}

void rujin::XInputGamepadSession::SetVibration(float vibration)
{
	XINPUT_VIBRATION vibrationState;
	vibration = std::clamp(vibration, 0.f, 1.f);

	ZeroMemory(&vibrationState, sizeof(XINPUT_VIBRATION));

	vibrationState.wLeftMotorSpeed = static_cast<WORD>(vibration * 65535);
	vibrationState.wRightMotorSpeed = static_cast<WORD>(vibration * 65535);

	LOG_INFO_("Left vibration: {} || Right vibration: {}", vibrationState.wLeftMotorSpeed, vibrationState.wRightMotorSpeed);

	XInputSetState(m_GamepadIndex, &vibrationState);
}

void rujin::XInputGamepadSession::SetLeftVibration(float leftVibration)
{
	XINPUT_VIBRATION vibrationState;
	leftVibration = std::clamp(leftVibration, 0.f, 1.f);

	ZeroMemory(&vibrationState, sizeof(XINPUT_VIBRATION));

	vibrationState.wLeftMotorSpeed = static_cast<WORD>(leftVibration * 65535);
	vibrationState.wRightMotorSpeed = 0;

	XInputSetState(m_GamepadIndex, &vibrationState);
}

void rujin::XInputGamepadSession::SetRightVibration(float rightVibration)
{
	XINPUT_VIBRATION vibrationState;
	rightVibration = std::clamp(rightVibration, 0.f, 1.f);

	ZeroMemory(&vibrationState, sizeof(XINPUT_VIBRATION));

	vibrationState.wLeftMotorSpeed = 0;
	vibrationState.wRightMotorSpeed = static_cast<WORD>(rightVibration * 65535);;

	XInputSetState(m_GamepadIndex, &vibrationState);
}
