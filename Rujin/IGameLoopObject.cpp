#include "RujinPCH.h"
#include "IGameLoopObject.h"

IGameLoopObject::~IGameLoopObject() = default;

void IGameLoopObject::SetEnabled(const bool enabled)
{
	if (m_IsEnabled == enabled) return;

	if (enabled) Enable();
	else Disable();
}

void IGameLoopObject::Enable()
{
	m_IsEnabled = true;
}

void IGameLoopObject::Disable()
{
	m_IsEnabled = false;
}

bool IGameLoopObject::IsEnabled() const
{
	return m_IsEnabled;
}
