#include "RujinPCH.h"

#include "GameObject.h"
#include "TransformComponent.h"

#include "EventData.h"

rujin::TransformComponent::TransformComponent()
	: Subject({})
{
}

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
	if (const Position localOffset{ pos - GetPosition() }; abs(localOffset.x) + abs(localOffset.y) > 2 * FLT_EPSILON)
	{
		m_LocalTransform.pos += localOffset;

		event::OnTransformChanged_t event(this);
		event.position = true;
		Notify(static_cast<uint32_t>(event::Identifier::OnTransformChanged), &event);
	}
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
	if (const Rotation localOffset = rot - GetRotation(); abs(localOffset) > FLT_EPSILON)
	{
		m_LocalTransform.rot += localOffset;

		event::OnTransformChanged_t event(this);
		event.rotation = true;
		Notify(static_cast<uint32_t>(event::Identifier::OnTransformChanged), &event);
	}
}

rujin::Scale rujin::TransformComponent::GetScale() const
{
	if (const auto* pParent = m_pGameObject->GetParent())
		return pParent->GetTransform()->GetScale() * GetLocalScale();

	return GetLocalScale();
}

void rujin::TransformComponent::SetScale(const Scale& scale)
{
	if (const Scale localOffset = scale - GetScale(); abs(scale.x) + abs(scale.y) > 2 * FLT_EPSILON )
	{
		m_LocalTransform.scale += localOffset;

		event::OnTransformChanged_t event(this);
		event.scale = true;
		Notify(static_cast<uint32_t>(event::Identifier::OnTransformChanged), &event);
	}
	
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

	event::OnTransformChanged_t event(this);
	event.position = true;
	Notify(static_cast<uint32_t>(event::Identifier::OnTransformChanged), &event);
}

void rujin::TransformComponent::AddLocalPosition(const Position& pos)
{
	if (abs(pos.x) + abs(pos.y) > 2 * FLT_EPSILON)
	{
		m_LocalTransform.pos += pos;

		event::OnTransformChanged_t event(this);
		event.position = true;
		Notify(static_cast<uint32_t>(event::Identifier::OnTransformChanged), &event);
	}
}

rujin::Rotation rujin::TransformComponent::GetLocalRotation() const
{
	return m_LocalTransform.rot;
}

void rujin::TransformComponent::SetLocalRotation(const Rotation rot)
{
	m_LocalTransform.rot = fmod(rot, std::numbers::pi_v<float> *2.f);

	event::OnTransformChanged_t event(this);
	event.rotation = true;
	Notify(static_cast<uint32_t>(event::Identifier::OnTransformChanged), &event);
}
void rujin::TransformComponent::AddLocalRotation(Rotation rot)
{
	if (abs(rot) > FLT_EPSILON)
	{
		m_LocalTransform.rot = fmod(m_LocalTransform.rot + rot, std::numbers::pi_v<float> *2.f);

		event::OnTransformChanged_t event(this);
		event.rotation = true;
		Notify(static_cast<uint32_t>(event::Identifier::OnTransformChanged), &event);
	}
}

const rujin::Scale& rujin::TransformComponent::GetLocalScale() const
{
	return m_LocalTransform.scale;
}

void rujin::TransformComponent::SetLocalScale(const Scale& scale)
{
	m_LocalTransform.scale = scale;

	event::OnTransformChanged_t event(this);
	event.scale = true;
	Notify(static_cast<uint32_t>(event::Identifier::OnTransformChanged), &event);
}
void rujin::TransformComponent::AddLocalScale(const Scale& scale)
{
	if (abs(scale.x) + abs(scale.y) > 2 * FLT_EPSILON)
	{
		m_LocalTransform.scale += scale;

		event::OnTransformChanged_t event(this);
		event.scale = true;
		Notify(static_cast<uint32_t>(event::Identifier::OnTransformChanged), &event);
	}
}

const rujin::Transform& rujin::TransformComponent::GetLocalTransform() const
{
	return m_LocalTransform;
}
