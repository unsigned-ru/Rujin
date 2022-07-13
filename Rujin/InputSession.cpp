#include "RujinPCH.h"
#include "InputSession.h"

#include "Rutils/General.h"

rujin::InputSession::InputSession(const InputDeviceType&& deviceType)
	: m_InputDeviceType{deviceType}
{
}