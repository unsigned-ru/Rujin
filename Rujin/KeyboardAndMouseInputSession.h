#pragma once
#include "InputSession.h"

namespace rujin
{
	class KeyboardAndMouseInputSession final : public InputSession
	{
	public:
		KeyboardAndMouseInputSession();
		~KeyboardAndMouseInputSession() override;

		KeyboardAndMouseInputSession(const KeyboardAndMouseInputSession&) = delete;
		KeyboardAndMouseInputSession(KeyboardAndMouseInputSession&&) noexcept = delete;
		KeyboardAndMouseInputSession operator=(const KeyboardAndMouseInputSession&) = delete;
		KeyboardAndMouseInputSession operator=(KeyboardAndMouseInputSession&&) noexcept = delete;

		bool IsMouseButtonDown(int button, bool previousFrame = false) const;
		bool IsKeyboardKeyDown(int key, bool previousFrame = false) const;

		POINT GetMousePosition(bool previousFrame = false) const;
		POINT GetMouseMovement() const;

		virtual bool UpdateStates() override;

	private:
		BYTE* m_pKeyboardState0{};
		BYTE* m_pKeyboardState1{};

		//point to one of the above
		BYTE* m_pCurrKeyboardState{};
		BYTE* m_pOldKeyboardState{};

		POINT m_OldMousePosition{};
		POINT m_CurrMousePosition{};
		POINT m_MouseMovement{};
	};
}
