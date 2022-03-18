#include "RujinPCH.h"

#include "GameLoop/GameObject.h"
#include "Components/TransformComponent.h"

//recursive
glm::vec3 rujin::TransformComponent::GetPosition() const
{
	if (const auto* pParent = m_pGameObject->GetParent())
		return pParent->GetTransform()->GetPosition() + GetLocalPosition();

	return GetLocalPosition();
}

void rujin::TransformComponent::SetPosition(const glm::vec2& posXY)
{
	//localOffset for global = requestedGlobal - lastLocal
	glm::vec3 lastLocal{};

	if (const auto* pParent = m_pGameObject->GetParent())
		lastLocal = pParent->GetTransform()->GetPosition();

	const glm::vec3 posXYZ{ posXY.x, posXY.y, lastLocal.z };
	m_localPosition = posXYZ - lastLocal;
}

void rujin::TransformComponent::SetPosition(const glm::vec3& posXYZ)
{
	glm::vec3 lastLocal{};
	if (const auto* pParent = m_pGameObject->GetParent())
		lastLocal = pParent->GetTransform()->GetPosition();

	m_localPosition = posXYZ - lastLocal;
}

glm::quat rujin::TransformComponent::GetRotation() const
{
	if (const auto* pParent = m_pGameObject->GetParent())
		return pParent->GetTransform()->GetRotation() * GetLocalRotation();

	return GetLocalRotation();
}

void rujin::TransformComponent::SetRotation(const glm::quat&)
{
	//TODO: implement
	// calculate local rotation offset to achieve said global rotation.
}

glm::vec3 rujin::TransformComponent::GetScale() const
{
	if (const auto* pParent = m_pGameObject->GetParent())
		return pParent->GetTransform()->GetScale() * GetLocalScale();

	return GetLocalScale();
}

void rujin::TransformComponent::SetScale(const glm::vec3&)
{
	//TODO: implement.
}

glm::vec3 rujin::TransformComponent::GetLocalPosition() const
{
	return m_localPosition;
}

glm::vec3& rujin::TransformComponent::GetLocalPosition()
{
	return m_localPosition;
}

void rujin::TransformComponent::SetLocalPosition(const glm::vec2& posXY)
{
	m_localPosition.x = posXY.x;
	m_localPosition.y = posXY.y;
}

void rujin::TransformComponent::SetLocalPosition(const glm::vec3& posXYZ)
{
	m_localPosition = posXYZ;
}

glm::quat rujin::TransformComponent::GetLocalRotation() const
{
	return m_localRotation;
}

glm::quat& rujin::TransformComponent::GetLocalRotation()
{
	return m_localRotation;
}

void rujin::TransformComponent::SetLocalRotation(const glm::quat& rot)
{
	m_localRotation = rot;
}

glm::vec3 rujin::TransformComponent::GetLocalScale() const
{
	return m_localScale;
}

glm::vec3& rujin::TransformComponent::GetLocalScale()
{
	return m_localScale;
}

void rujin::TransformComponent::SetLocalScale(const glm::vec3& scale)
{
	m_localScale = scale;
}
