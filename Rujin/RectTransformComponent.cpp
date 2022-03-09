#include "RujinPCH.h"
#include "RectTransformComponent.h"

glm::vec2 rujin::RectTransformComponent::GetSize() const
{
	return m_Size;
}

void rujin::RectTransformComponent::SetSize(const glm::vec2& size)
{
	m_Size = size;
}

glm::vec2 rujin::RectTransformComponent::GetPivot() const
{
	return m_Pivot;
}

void rujin::RectTransformComponent::SetPivot(const glm::vec2& size)
{
	m_Size = size;
}
