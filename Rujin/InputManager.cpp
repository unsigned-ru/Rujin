#include "RujinPCH.h"
#include "InputManager.h"

#include "PlayerInput.h"
#include "Rutils/DeltaTimer.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Xinput.h>
#pragma comment(lib, "xinput.lib")

#include <array>
#include <numeric>
#include <queue>

namespace rujin::input
{
	class InputManager::InputManagerXInputImpl final
	{
	public:
		InputManager::InputManagerXInputImpl()
			: m_DeviceScanTimer{s_DeviceScanDelay, true}
			, m_AvailableGamepads{}
		{
			//Set up iota data for player index queue.
			std::array<unsigned char, s_MaxPlayerCount> tempArr{};
			std::iota(tempArr.begin(), tempArr.end(), 0);

			//create player index queue
			m_PlayerIndexQueue = std::priority_queue<PlayerIndex, std::vector<PlayerIndex>, std::greater<PlayerIndex>>(tempArr.begin(), tempArr.end());
		}

		void ProcessInput(float deltaTime)
		{
			//device scanning. (every 3 seconds)
			if (m_DeviceScanTimer.Tick(deltaTime))
				RegisterNewDevices();

			//update connected devices
			for (auto it = m_Players.begin(); it != m_Players.end(); ++it)
			{
				it->second.Update();
			//	if (!pSession->UpdateStates())
			//	{
			//		//Tried to update device, but it lost connection.
			//		//handle disconnect.

			//		//add player index back into the pool
			//		m_PlayerIndexQueue.push(pSession->GetPlayerIndex());

			//		switch (pSession->GetInputDeviceType())
			//		{
			//			case InputDeviceType::KeyboardAndMouse:
			//			{
			//				if (dynamic_cast<KeyboardAndMouseInputSession*>(pSession))
			//				{
			//					//mark keyboard as inactive
			//					m_pKeyboardSession = nullptr;
			//				}
			//				break;
			//			}
			//			case InputDeviceType::Gamepad:
			//			{
			//				if (GamepadInputSession* pGamepadSession = dynamic_cast<GamepadInputSession*>(pSession))
			//				{
			//					//mark keyboard as inactive
			//					m_ConnectedGamepads[pGamepadSession->GetGamepadIndex()] = false;
			//				}
			//				break;
			//			}
			//		}

			//		//remove session from list.
			//		it = m_pInputSessions.erase(it);
			//	}
			}
		}

		PlayerIndex RegisterPlayer() 
		{
			//get next player index
			PlayerIndex playerIndex = m_PlayerIndexQueue.top();
			m_PlayerIndexQueue.pop();

			//Create player.
			PlayerInput player = PlayerInput(playerIndex);

			return playerIndex;
		}

		void AddInputAction(PlayerIndex player, uint32_t inputAction, const InputActionKeybinds& keybinds)
		{
			auto it = m_Players.find(player);
			
			if (it != m_Players.end())
				it->second.AddInputAction(inputAction, keybinds);
		}

		void AddAxisAction(PlayerIndex player, uint32_t axisAction, const AxisActionKeybinds& keybinds)
		{
			auto it = m_Players.find(player);

			if (it != m_Players.end())
				it->second.AddAxisAction(axisAction, keybinds);
		}

	private:
		std::unordered_map<PlayerIndex, PlayerInput> m_Players;

		rutils::DeltaTimer m_DeviceScanTimer;
		KeyboardAndMouseInputSession* m_pKeyboardSession = nullptr;

		GamepadAvailability m_AvailableGamepads[XUSER_MAX_COUNT];
		std::priority_queue<PlayerIndex, std::vector<PlayerIndex>, std::greater<PlayerIndex>> m_PlayerIndexQueue;

		void RegisterNewDevices()
		{
			//check if any Players need an input device.
			for (auto& pair : m_Players)
			{
				PlayerInput& player = pair.second;
				if (!player.HasInputDevice())
				{
					//check if we have keyboard and mouse available.
					if ((!m_pKeyboardSession) && GetKeyboardState(nullptr))
					{
						//keyboard is available, create session and assign to player.
						m_pKeyboardSession = new KeyboardAndMouseInputSession();
			
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
							player.RegisterInputSession(new GamepadInputSession(i, state));
							gamepad.isInUse = true; //mark gamepad as in-use.
						}
					}
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


	PlayerIndex rujin::input::InputManager::RegisterPlayer()
	{
		return m_pImpl->RegisterPlayer();
	}

	void rujin::input::InputManager::AddInputAction(PlayerIndex player, uint32_t inputAction, const InputActionKeybinds& keybinds)
	{
		m_pImpl->AddInputAction(player, inputAction, keybinds);
	}

	void rujin::input::InputManager::AddAxisAction(PlayerIndex player, uint32_t axisAction, const AxisActionKeybinds& keybinds)
	{
		m_pImpl->AddAxisAction(player, axisAction, keybinds);
	}

#pragma endregion
}