#include "RujinPCH.h"
#include "TransformComponent.h"

rujin::TransformComponent::TransformComponent(const std::weak_ptr<GameObject> gameObject)
	: Component(gameObject)
{
}

const glm::vec3& rujin::TransformComponent::GetPosition() const
{
	return m_Position;
}

glm::vec3& rujin::TransformComponent::GetPosition()
{
	return m_Position;
}

const glm::vec3& rujin::TransformComponent::GetScale() const
{
	return m_Scale;
}

glm::vec3& rujin::TransformComponent::GetScale()
{
	return m_Scale;
}

void rujin::TransformComponent::SetPosition(const glm::vec3& pos)
{
	m_Position = pos;
}

void rujin::TransformComponent::SetPosition(const glm::vec2& posXY)
{
	m_Position.x = posXY.x;
	m_Position.y = posXY.y;
}

const glm::quat& rujin::TransformComponent::GetRotation() const
{
	return m_Rotation;
}

glm::quat& rujin::TransformComponent::GetRotation()
{
	return m_Rotation;
}

void rujin::TransformComponent::SetRotation(const glm::quat& rot)
{
	m_Rotation = rot;
}

void rujin::TransformComponent::SetScale(const glm::vec3& scale)
{
	m_Scale = scale;
}
