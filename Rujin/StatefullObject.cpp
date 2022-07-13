#include "RujinPCH.h"
#include "StatefullObject.h"

StatefullObject& StatefullObject::operator=(StatefullObject&& other) noexcept = default;
StatefullObject& StatefullObject::operator=(const StatefullObject& other) = default;

void StatefullObject::SetEnabled(const bool enabled)
{
	if (enabled)
		Enable();
	else
		Disable();
}

void StatefullObject::Enable()
{
	if (m_IsEnabled)
		return;

	OnEnable();
	m_IsEnabled = true;
}

void StatefullObject::Disable()
{
	if (!m_IsEnabled)
		return;

	OnDisable();
	m_IsEnabled = false;
}

bool StatefullObject::IsEnabled() const
{
	return m_IsEnabled;
}
