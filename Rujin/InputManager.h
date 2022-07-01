#pragma once
#include <memory>
#include "Singleton.h"
#include <variant>
#include "InputStructs.h"

namespace rujin
{
	using InputButton = std::variant<GamepadButton, KeyboardAndMouseButton>;

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
			~InputManager() override;

			InputManager(InputManager&) = delete;
			InputManager& operator=(InputManager&) = delete;
			InputManager(InputManager&&) = delete;
			InputManager& operator=(InputManager&&) = delete;

			void ProcessInput(float deltaTime) const;

			void RegisterKeybind(InputButton button, ButtonState triggerState, std::unique_ptr<command::IBase>&& pCommand, PlayerIndex playerIndex = 0);
			
			glm::vec2& GetMousePosition() const;
			glm::vec2& GetMouseMovement() const;

			InputSession* GetPlayerInputSession(PlayerIndex playerIndex = 0) const;

		private:
			class InputManagerImpl;
			std::unique_ptr<InputManagerImpl> m_pImpl;
		};

	}
}
