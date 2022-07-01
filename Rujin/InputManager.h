#pragma once
#include <memory>
#include "Singleton.h"
#include "InputStructs.h"

namespace rujin
{
	namespace command
	{
		class IBase;
	};

	namespace input
	{

		class InputManager final : public Singleton<InputManager>
		{
			inline static constexpr float s_DeviceScanDelay{ 3.f };
			inline static constexpr unsigned char s_MaxPlayerCount{ 4 };

		public:
			InputManager();
			~InputManager();

			InputManager(InputManager&) = delete;
			InputManager& operator=(InputManager&) = delete;
			InputManager(InputManager&&) = delete;
			InputManager& operator=(InputManager&&) = delete;

			void ProcessInput(float deltaTime) const;

			//glm::vec2& GetMousePosition() const;
			//glm::vec2& GetMouseMovement() const;

			PlayerIndex RegisterPlayer() const;

			void AddInputAction(PlayerIndex player, uint32_t inputAction, const InputActionKeybinds& keybinds) const;
			void AddAxisAction(PlayerIndex player, uint32_t axisAction, const AxisActionKeybinds& keybinds) const;

			bool IsInputActionTriggered(PlayerIndex player, uint32_t inputAction) const;
			bool IsAxisActionTriggered(PlayerIndex player, uint32_t axisAction) const;

		private:
			class InputManagerXInputImpl;
			std::unique_ptr<InputManagerXInputImpl> m_pImpl;
		};

	}
}
