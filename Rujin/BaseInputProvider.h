#ifndef BASE_INPUT_PROVIDER_H
#define BASE_INPUT_PROVIDER_H
#include <queue>
#include <unordered_map>

#include "InputService.h"
#include "XInputKeyboardAndMouseSession.h"
#include "Rutils/DeltaTimer.h"

namespace rujin
{
	class PlayerInput;

	//Implements non-lib-specific input logic.
	class BaseInputProvider : public InputService
	{
		inline static constexpr float s_DeviceScanDelay{ 3.f };
		inline static constexpr unsigned char s_MaxPlayerCount{ 4 };

	public:
		BaseInputProvider();
		~BaseInputProvider() override;

		BaseInputProvider(const BaseInputProvider&) = delete;
		BaseInputProvider(BaseInputProvider&&) noexcept = delete;
		BaseInputProvider& operator=(const BaseInputProvider&) = delete;
		BaseInputProvider& operator=(BaseInputProvider&&) noexcept = delete;

		void ProcessInput(float deltaTime) override;

		PlayerIndex RegisterPlayer() override;

		void AddInputAction(PlayerIndex player, uint32_t inputAction, const InputActionKeybinds& keybinds) override;
		void AddAxisAction(PlayerIndex player, uint32_t axisAction, const AxisActionKeybinds& keybinds) override;

		void SetLeftVibration(PlayerIndex player, float intensity) override;
		void SetRightVibration(PlayerIndex player, float intensity) override;
		void SetVibration(PlayerIndex player, float intensity) override;

		bool IsInputActionTriggered(PlayerIndex player, uint32_t inputAction) const override;
		bool IsAxisActionTriggered(PlayerIndex player, uint32_t axisAction, float* pIntensity) const override;

		POINT GetMouseMovement() const override;
		POINT GetMousePosition(bool previousFrame) const override;

	protected:
		virtual void RegisterNewDevices() = 0;

		std::unordered_map<PlayerIndex, PlayerInput> m_Players;

		rutils::DeltaTimer m_DeviceScanTimer;
		XInputKeyboardAndMouseSession* m_pKeyboardSession = nullptr;

		std::priority_queue<PlayerIndex, std::vector<PlayerIndex>, std::greater<PlayerIndex>> m_PlayerIndexQueue;
	};

}


#endif // Include Guard: BASE_INPUT_PROVIDER_H
