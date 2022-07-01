#include "RujinPCH.h"
#include "InputManager.h"
#include "Commands.h"
#include "InputSession.h"
#include "Rutils/DeltaTimer.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Xinput.h>
#pragma comment(lib, "xinput.lib")

#include <unordered_map>
#include <queue>
#include <numeric>
#include <array>


namespace rujin::input
{
	class InputManager::InputManagerXInputImpl final
	{
	public:
		InputManager::InputManagerXInputImpl()
			: m_DeviceScanTimer{s_DeviceScanDelay, true}
		{
			//make sure all booleans default to false.
			ZeroMemory(&m_ConnectedGamepads, sizeof(m_ConnectedGamepads));

			//Set up ioata data for player index queue.
			std::array<unsigned char, s_MaxPlayerCount> tempArr{};
			std::iota(tempArr.begin(), tempArr.end(), 0);

			//create player index queue
			m_PlayerIndexQueue = std::priority_queue<PlayerIndex, std::greater<PlayerIndex>>(tempArr.begin(), tempArr.end());

			//Register devices
			RegisterNewDevices();
		}

		void ProcessInput(float deltaTime)
		{
			//device scanning. (every 3 seconds)
			if (m_DeviceScanTimer.Tick(deltaTime))
				RegisterNewDevices();

			//update connected devices
			for (auto it = m_pInputSessions.begin(); it != m_pInputSessions.end(); ++it)
			{
				InputSession* pSession = it->second;
				if (!pSession->UpdateStates())
				{
					//Tried to update device, but it lost connection.
					//handle disconnect.

					//add player index back into the pool
					m_PlayerIndexQueue.push(pSession->GetPlayerIndex());

					switch (pSession->GetInputDeviceType())
					{
						case InputDeviceType::KeyboardAndMouse:
						{
							if (dynamic_cast<KeyboardAndMouseInputSession*>(pSession))
							{
								//mark keyboard as inactive
								m_pKeyboardSession = nullptr;
							}
							break;
						}
						case InputDeviceType::Gamepad:
						{
							if (GamepadInputSession* pGamepadSession = dynamic_cast<GamepadInputSession*>(pSession))
							{
								//mark keyboard as inactive
								m_ConnectedGamepads[pGamepadSession->GetGamepadIndex()] = false;
							}
							break;
						}
					}

					//remove session from list.
					it = m_pInputSessions.erase(it);
				}
			}
		}
		InputSession* GetPlayerInputSession(PlayerIndex playerIndex)
		{
			auto it = m_pInputSessions.find(playerIndex);

			if (it != m_pInputSessions.end())
				return it->second;
			else 
				return nullptr;
		}

	private:
		rutils::DeltaTimer m_DeviceScanTimer;
		KeyboardAndMouseInputSession* m_pKeyboardSession = nullptr;

		bool m_ConnectedGamepads[XUSER_MAX_COUNT];
		std::priority_queue<PlayerIndex, std::greater<PlayerIndex>> m_PlayerIndexQueue;

		std::unordered_map<PlayerIndex, InputSession*> m_pInputSessions{};

		void RegisterNewDevices()
		{
			if (m_PlayerIndexQueue.empty())
			{
				LOG_WARNING("New input device detected, but maximum allowed input devices exceeded.");
				return;
			}

			if ((!m_pKeyboardSession) && GetKeyboardState(nullptr))
			{
				PlayerIndex playerIndex = m_PlayerIndexQueue.top();
				auto* kbSession = new KeyboardAndMouseInputSession(playerIndex);
				m_pInputSessions.insert(std::make_pair(playerIndex, kbSession));
				m_PlayerIndexQueue.pop();
				m_pKeyboardSession = kbSession;
			}

			for (DWORD i = 0; i < XUSER_MAX_COUNT; ++i)
			{
				if (m_PlayerIndexQueue.empty())
				{
					LOG_WARNING("New input device detected, but maximum allowed input devices exceeded.");
					return;
				}

				//check if got connected
				XINPUT_STATE state;
				ZeroMemory(&state, sizeof(XINPUT_STATE));

				if (!m_ConnectedGamepads[i] && XInputGetState(i, &state) == ERROR_SUCCESS)
				{
					//create gamepad input session with correct ids.
					PlayerIndex playerIndex = m_PlayerIndexQueue.top();
					m_pInputSessions.insert(std::make_pair(playerIndex, new GamepadInputSession(m_PlayerIndexQueue.top(), i, state)));
					m_PlayerIndexQueue.pop();
					m_ConnectedGamepads[i] = true;
				}
			}
		}
	};

	//ctor and dtor -- NEED to be declared AFTER the implementation class definition in .cpp
	InputManager::InputManager() : m_pImpl(std::make_unique<InputManagerXInputImpl>()) {}
	InputManager::~InputManager() = default;

#pragma region pImpl_Propagation
	void InputManager::ProcessInput(float deltaTime) const
	{
		m_pImpl->ProcessInput(deltaTime);
	}

	InputSession* InputManager::GetPlayerInputSession(PlayerIndex playerIndex) const
	{
		return m_pImpl->GetPlayerInputSession(playerIndex);
	}

#pragma endregion
}