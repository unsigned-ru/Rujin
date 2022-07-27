#pragma once

namespace rujin
{
	enum class InputDeviceType : int
	{
		KeyboardAndMouse,
		Gamepad
	};

	class InputSession
	{
	public:
		explicit InputSession(const InputDeviceType&& deviceType);
		virtual ~InputSession() = default;

		InputSession(const InputSession&) = delete;
		InputSession(InputSession&&) noexcept = delete;
		InputSession& operator=(const InputSession&) = delete;
		InputSession& operator=(InputSession&&) noexcept = delete;

		//returns false if input device disconnects.
		virtual bool UpdateStates() = 0;

		InputDeviceType GetInputDeviceType() const { return m_InputDeviceType; };

	private:
		InputDeviceType m_InputDeviceType;
	};
}