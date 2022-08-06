#ifndef XINPUT_KEYBOARD_AND_MOUSE_SESSION_H
#define XINPUT_KEYBOARD_AND_MOUSE_SESSION_H

#include "KeyboardAndMouseInputSession.h"

namespace rujin
{
	class XInputKeyboardAndMouseSession final : public KeyboardAndMouseInputSession
	{
	public:
		XInputKeyboardAndMouseSession();
		~XInputKeyboardAndMouseSession() override;

		XInputKeyboardAndMouseSession(const XInputKeyboardAndMouseSession&) = delete;
		XInputKeyboardAndMouseSession(XInputKeyboardAndMouseSession&&) noexcept = delete;
		XInputKeyboardAndMouseSession operator=(const XInputKeyboardAndMouseSession&) = delete;
		XInputKeyboardAndMouseSession operator=(XInputKeyboardAndMouseSession&&) noexcept = delete;

		bool IsMouseButtonDown(int button, bool previousFrame = false) const override;
		bool IsKeyboardKeyDown(int key, bool previousFrame = false) const override;

		glm::ivec2 GetMousePosition(bool previousFrame = false) const override;
		glm::ivec2 GetMouseMovement() const override;

		bool UpdateStates() override;

	private:
		BYTE* m_pKeyboardState0{};
		BYTE* m_pKeyboardState1{};

		//point to one of the above
		BYTE* m_pCurrKeyboardState{};
		BYTE* m_pOldKeyboardState{};

		glm::ivec2 m_OldMousePosition{};
		glm::ivec2 m_CurrMousePosition{};
		glm::ivec2 m_MouseMovement{};
	};
}


#endif // Include Guard: XINPUT_KEYBOARD_AND_MOUSE_SESSION_H
