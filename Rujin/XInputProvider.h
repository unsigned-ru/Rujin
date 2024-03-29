﻿#ifndef XINPUT_PROVIDER_H
#define XINPUT_PROVIDER_H


#include "BaseInputProvider.h"
#include "InputService.h"

namespace rujin
{
	class XInputProvider final : public BaseInputProvider
	{
	public:
		XInputProvider();
		~XInputProvider() override = default;

		XInputProvider(const XInputProvider&) = delete;
		XInputProvider(XInputProvider&&) noexcept = delete;
		XInputProvider& operator=(const XInputProvider&) = delete;
		XInputProvider& operator=(XInputProvider&&) noexcept = delete;

	protected:
		void RegisterNewDevices() override;

	private:
		GamepadAvailability m_AvailableGamepads[XUSER_MAX_COUNT]{};
	};
}


#endif // Include Guard: XINPUT_PROVIDER_H
