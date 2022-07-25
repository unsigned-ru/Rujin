#include "RujinPCH.h"
#include "Camera.h"

rujin::Camera::Camera(const glm::ivec2& size, const Transform& transform)
	: m_Size(size)
	, m_Transform(transform) {}

void rujin::Camera::Project() const
{
	glTranslatef(-m_Transform.pos.x, -m_Transform.pos.y, 0);
	glRotatef(m_Transform.rot, 0.f, 0.f, 1.f);
}

void rujin::Camera::SetPosition(const Position& pos)
{
	m_Transform.pos = pos;
}

void rujin::Camera::SetRotation(Rotation rot)
{
	m_Transform.rot = std::clamp(rot, -360.f, 360.f);
}

const rujin::Position& rujin::Camera::GetPosition() const
{
	return m_Transform.pos;
}

rujin::Rotation rujin::Camera::GetRotation() const
{
	return m_Transform.rot;
}

const glm::vec2& rujin::Camera::GetSize() const
{
	return m_Size;
}
