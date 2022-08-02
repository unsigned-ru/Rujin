#include "RujinPCH.h"
#include "Collider.h"

rujin::Collider::Collider(Component* pComponent, const bool isStatic, const glm::vec2& pivot)
	: m_pComponent(pComponent)
	, m_Pivot(pivot)
	, m_IsStatic(isStatic)
{
}

const glm::vec2& rujin::Collider::GetPivot() const
{
	return m_Pivot;
}

void rujin::Collider::SetPivot(const glm::vec2& pivot)
{
	m_Pivot = pivot;
}

bool rujin::Collider::IsStatic() const
{
	return m_IsStatic;
}

rujin::Component* rujin::Collider::GetComponent() const
{
	return m_pComponent;
}
