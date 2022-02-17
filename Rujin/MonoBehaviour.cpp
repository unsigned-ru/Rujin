#include "RujinPCH.h"
#include "MonoBehaviour.h"

void MonoBehaviour::SetEnabled(const bool enabled)
{
	if (m_IsEnabled == enabled) return;

	if (enabled) Enable();
	else Disable();
}

void MonoBehaviour::Enable()
{
	m_IsEnabled = true;
}

void MonoBehaviour::Disable()
{
	m_IsEnabled = false;
}

bool MonoBehaviour::IsEnabled() const
{
	return m_IsEnabled;
}
