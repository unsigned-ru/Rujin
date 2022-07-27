#pragma once
#include <unordered_map>
#include <Xinput.h>

#include "InputStructs.h"
#include "Service.h"

namespace rujin
{
	class InputService : public Service
	{
	public:
		InputService() = default;
		~InputService() override = default;

		InputService(InputService&) = delete;
		InputService& operator=(InputService&) = delete;
		InputService(InputService&&) = delete;
		InputService& operator=(InputService&&) = delete;

		virtual void ProcessInput(float deltaTime) = 0;

		virtual POINT GetMousePosition(bool previousFrame = false) const = 0;
		virtual POINT GetMouseMovement() const = 0;

		virtual PlayerIndex RegisterPlayer() = 0;

		virtual void AddInputAction(PlayerIndex player, uint32_t inputAction, const InputActionKeybinds& keybinds) = 0;
		virtual void AddAxisAction(PlayerIndex player, uint32_t axisAction, const AxisActionKeybinds& keybinds) = 0;
		virtual void SetLeftVibration(PlayerIndex player, float intensity) = 0;
		virtual void SetRightVibration(PlayerIndex player, float intensity) = 0;
		virtual void SetVibration(PlayerIndex player, float intensity) = 0;

		virtual bool IsInputActionTriggered(PlayerIndex player, uint32_t inputAction) const = 0;
		virtual bool IsAxisActionTriggered(PlayerIndex player, uint32_t axisAction, float* pIntensity = nullptr) const = 0;
	};
}
