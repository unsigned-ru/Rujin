#include "RujinPCH.h"
#include "KeyboardAndMouseInputSession.h"

rujin::KeyboardAndMouseInputSession::KeyboardAndMouseInputSession()
	: InputSession(InputDeviceType::KeyboardAndMouse)
{
	m_pKeyboardState0 = new BYTE[256];
	m_pKeyboardState1 = new BYTE[256];

	ZeroMemory(m_pKeyboardState0, sizeof(BYTE) * 256);
	ZeroMemory(m_pKeyboardState1, sizeof(BYTE) * 256);

	m_pCurrKeyboardState = m_pKeyboardState1;
	m_pOldKeyboardState = m_pKeyboardState0;
}

rujin::KeyboardAndMouseInputSession::~KeyboardAndMouseInputSession()
{
	delete[] m_pKeyboardState0;
	delete[] m_pKeyboardState1;

	m_pKeyboardState0 = nullptr;
	m_pKeyboardState1 = nullptr;
	m_pCurrKeyboardState = nullptr;
	m_pOldKeyboardState = nullptr;
}


bool rujin::KeyboardAndMouseInputSession::IsMouseButtonDown(const int button, const bool previousFrame) const
{
	// Note: mouse and keyboard button detection are stored in the same state.
	// This function is only there for user clarity
	return IsKeyboardKeyDown(button, previousFrame); 
}

bool rujin::KeyboardAndMouseInputSession::IsKeyboardKeyDown(const int key, const bool previousFrame) const
{
	if (key == -1)
		return false;

	const BYTE* pState = previousFrame ? m_pOldKeyboardState : m_pCurrKeyboardState;

	return (pState[key] & 0xF0) != 0;
}

POINT rujin::KeyboardAndMouseInputSession::GetMousePosition(const bool previousFrame) const
{
	return previousFrame ? m_OldMousePosition : m_CurrMousePosition;
}

POINT rujin::KeyboardAndMouseInputSession::GetMouseMovement() const
{
	return m_MouseMovement;
}

bool rujin::KeyboardAndMouseInputSession::UpdateStates()
{
	//swap buffers
	BYTE* temp = m_pOldKeyboardState;
	m_pOldKeyboardState = m_pCurrKeyboardState;
	m_pCurrKeyboardState = temp;

	if (!GetKeyboardState(m_pCurrKeyboardState))
		return false; // kb disconnected or something went wrong.

	//Update mouse position
	m_OldMousePosition = m_CurrMousePosition;
	GetCursorPos(&m_CurrMousePosition);

	m_MouseMovement.x = m_CurrMousePosition.x - m_OldMousePosition.x;
	m_MouseMovement.y = m_CurrMousePosition.y - m_OldMousePosition.y;

	return true;
}
