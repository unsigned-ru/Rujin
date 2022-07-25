#include "RujinPCH.h"

#include "GameObject.h"
#include "TransformComponent.h"

//recursive
rujin::Position rujin::TransformComponent::GetPosition() const
{
	if (const auto* pParent = m_pGameObject->GetParent())
		return pParent->GetTransform()->GetPosition() + GetLocalPosition();

	return GetLocalPosition();
}

void rujin::TransformComponent::SetPosition(const rujin::Position& pos)
{
	//localOffset for global = newGlobal - global
	m_LocalTransform.pos += pos - GetPosition();
}

rujin::Rotation rujin::TransformComponent::GetRotation() const
{
	if (const auto* pParent = GameObject()->GetParent())
		return fmod(pParent->GetTransform()->GetRotation() + GetLocalRotation(), 2.f * std::numbers::pi_v<float>);

	return GetLocalRotation();
}

void rujin::TransformComponent::SetRotation(const Rotation rot)
{
	//calculate difference between  wanted global rotation and current global rotation
	//and add locally
	m_LocalTransform.rot += rot - GetRotation();
}

rujin::Scale rujin::TransformComponent::GetScale() const
{
	if (const auto* pParent = m_pGameObject->GetParent())
		return pParent->GetTransform()->GetScale() * GetLocalScale();

	return GetLocalScale();
}

void rujin::TransformComponent::SetScale(const Scale& scale)
{
	m_LocalTransform.scale += scale - GetScale();
}

rujin::Transform rujin::TransformComponent::GetTransform() const
{
	if (const auto* pParent = m_pGameObject->GetParent())
	{
		return pParent->GetTransform()->GetTransform() + GetLocalTransform();
	}

	return GetLocalTransform();
}

const rujin::Position& rujin::TransformComponent::GetLocalPosition() const
{
	return m_LocalTransform.pos;
}

void rujin::TransformComponent::SetLocalPosition(const Position& pos)
{
	m_LocalTransform.pos = pos;
}

rujin::Rotation rujin::TransformComponent::GetLocalRotation() const
{
	return m_LocalTransform.rot;
}

void rujin::TransformComponent::SetLocalRotation(const Rotation rot)
{
	m_LocalTransform.rot = fmod(rot, std::numbers::pi_v<float> *2.f);
}

const rujin::Scale& rujin::TransformComponent::GetLocalScale() const
{
	return m_LocalTransform.scale;
}

void rujin::TransformComponent::SetLocalScale(const Scale& scale)
{
	m_LocalTransform.scale = scale;
}

const rujin::Transform& rujin::TransformComponent::GetLocalTransform() const
{
	return m_LocalTransform;
}
