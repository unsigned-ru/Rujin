#include "RujinPCH.h"
#include "XInputKeyboardAndMouseSession.h"

#include "Rujin.h"

rujin::XInputKeyboardAndMouseSession::XInputKeyboardAndMouseSession()
	: KeyboardAndMouseInputSession()
{
	m_pKeyboardState0 = new BYTE[256];
	m_pKeyboardState1 = new BYTE[256];

	ZeroMemory(m_pKeyboardState0, sizeof(BYTE) * 256);
	ZeroMemory(m_pKeyboardState1, sizeof(BYTE) * 256);

	m_pCurrKeyboardState = m_pKeyboardState1;
	m_pOldKeyboardState = m_pKeyboardState0;
}

rujin::XInputKeyboardAndMouseSession::~XInputKeyboardAndMouseSession()
{
	delete[] m_pKeyboardState0;
	delete[] m_pKeyboardState1;

	m_pKeyboardState0 = nullptr;
	m_pKeyboardState1 = nullptr;
	m_pCurrKeyboardState = nullptr;
	m_pOldKeyboardState = nullptr;
}


bool rujin::XInputKeyboardAndMouseSession::IsMouseButtonDown(const int button, const bool previousFrame) const
{
	// Note: mouse and keyboard button detection are stored in the same state.
	// This function is only there for user clarity
	return IsKeyboardKeyDown(button, previousFrame); 
}

bool rujin::XInputKeyboardAndMouseSession::IsKeyboardKeyDown(const int key, const bool previousFrame) const
{
	if (key == -1)
		return false;

	const BYTE* pState = previousFrame ? m_pOldKeyboardState : m_pCurrKeyboardState;

	return (pState[key] & 0xF0) != 0;
}

glm::ivec2 rujin::XInputKeyboardAndMouseSession::GetMousePosition(const bool previousFrame) const
{
	return previousFrame ? m_OldMousePosition : m_CurrMousePosition;
}

glm::ivec2 rujin::XInputKeyboardAndMouseSession::GetMouseMovement() const
{
	return m_MouseMovement;
}

bool rujin::XInputKeyboardAndMouseSession::UpdateStates()
{
	//swap buffers
	BYTE* temp = m_pOldKeyboardState;
	m_pOldKeyboardState = m_pCurrKeyboardState;
	m_pCurrKeyboardState = temp;

	if (!GetKeyboardState(m_pCurrKeyboardState))
		return false; // kb disconnected or something went wrong.

	//Update mouse position
	m_OldMousePosition = m_CurrMousePosition;

	SDL_GetMouseState(&m_CurrMousePosition.x, &m_CurrMousePosition.y);
	m_CurrMousePosition.y = Rujin::Get()->GetWindowContext().windowSize.y - m_CurrMousePosition.y;
	

	m_MouseMovement = m_CurrMousePosition - m_OldMousePosition;

	return true;
}
