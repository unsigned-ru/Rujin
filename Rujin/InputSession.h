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

		//returns false if input device disconnects.
		virtual bool UpdateStates() = 0;

		InputDeviceType GetInputDeviceType() const { return m_InputDeviceType; };

	private:
		InputDeviceType m_InputDeviceType;
	};
}