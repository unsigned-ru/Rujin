#include "RujinPCH.h"
#include "XInputProvider.h"

#include <ranges>

#include "XInputGamepadSession.h"
#include "PlayerInput.h"

rujin::XInputProvider::XInputProvider()
	: BaseInputProvider()
{}

void rujin::XInputProvider::RegisterNewDevices()
{
	//check if any Players need an input device.
	for (auto& player : m_Players | std::views::values)
	{
		if (!player.HasInputDevice())
		{
			//check if we have keyboard and mouse available.
			if (!m_pKeyboardSession)
				//TODO: this assumes you always have a keyboard and mouse. Make it detect whether you have or not.
			{
				//keyboard is available, create session and assign to player.
				m_pKeyboardSession = new XInputKeyboardAndMouseSession();

				player.RegisterInputSession(m_pKeyboardSession);
				continue;
			}

			//check if we have a gamepad available
			for (GamepadIndex i = 0; i < XUSER_MAX_COUNT; ++i)
			{
				GamepadAvailability& gamepad = m_AvailableGamepads[i];

				XINPUT_STATE state;
				ZeroMemory(&state, sizeof(XINPUT_STATE));

				//check if gamepad got connected
				if (!gamepad.isConnected && XInputGetState(i, &state) == ERROR_SUCCESS)
					gamepad.isConnected = true;

				//check if gamped is connected and available.
				if (gamepad.isConnected && !gamepad.isInUse)
				{
					//we found an availble gamepad, create input session and assign to player
					player.RegisterInputSession(new XInputGamepadSession(i, state));
					gamepad.isInUse = true; //mark gamepad as in-use.
				}
			}
		}
	}
}
