#ifndef KEYBOARD_AND_MOUSE_INPUT_SESSION_H
#define KEYBOARD_AND_MOUSE_INPUT_SESSION_H

#include "InputSession.h"

namespace rujin
{
	class KeyboardAndMouseInputSession : public InputSession
	{
	public:
		KeyboardAndMouseInputSession() : InputSession(InputDeviceType::KeyboardAndMouse) {}

		virtual bool IsMouseButtonDown(int button, bool previousFrame = false) const = 0;
		virtual bool IsKeyboardKeyDown(int key, bool previousFrame = false) const = 0;

		virtual glm::ivec2 GetMousePosition(bool previousFrame = false) const = 0;
		virtual glm::ivec2 GetMouseMovement() const = 0;

		virtual bool UpdateStates() override = 0;
	};
}


#endif // Include Guard: KEYBOARD_AND_MOUSE_INPUT_SESSION_H
