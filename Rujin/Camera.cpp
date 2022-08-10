#include "RujinPCH.h"
#include "Camera.h"

rujin::Camera::Camera(const glm::ivec2& size)
	: m_Size(size)
	, m_Transform{nullptr} {}

void rujin::Camera::Project() const
{
	glTranslatef(-m_Transform.GetPosition().x, -m_Transform.GetPosition().y, 0);
	glRotatef(m_Transform.GetRotation(), 0.f, 0.f, 1.f);
}

rujin::Transform& rujin::Camera::GetTransform()
{
	return m_Transform;
}

const glm::vec2& rujin::Camera::GetSize() const
{
	return m_Size;
}
