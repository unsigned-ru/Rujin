#include "RujinPCH.h"
#include "Transform.h"

rujin::Transform::Transform(const glm::vec3& pos)
	: m_Position{pos}
{

}

void rujin::Transform::SetPosition(const float x, const float y, const float z)
{
	m_Position.x = x;
	m_Position.y = y;
	m_Position.z = z;
}
