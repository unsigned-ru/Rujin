#include "RujinPCH.h"

#include "GameObject.h"
#include "TransformComponent.h"

#include "EventData.h"

#include "Rutils/Math.h"

rujin::TransformComponent::TransformComponent()
	: Subject({})
{
}

const rujin::Position& rujin::TransformComponent::GetPosition() const
{
	return m_GlobalTransform.pos;
}

void rujin::TransformComponent::SetPosition(const rujin::Position& pos)
{
	//localOffset for global = newGlobal - global
	if (const Position localOffset{ pos - GetPosition() }; abs(localOffset.x) + abs(localOffset.y) > 2 * FLT_EPSILON)
	{
		m_LocalTransform.pos += localOffset;

		m_TransformChanged |= TransformChanged::POSITION;
	}
}

rujin::Rotation rujin::TransformComponent::GetRotation() const
{
	return m_GlobalTransform.rot;
}


void rujin::TransformComponent::SetRotation(const Rotation rot)
{
	//calculate difference between requested global rotation and current global rotation
	//and add locally
	if (const Rotation localOffset = rot - GetRotation(); abs(localOffset) > FLT_EPSILON)
	{
		m_LocalTransform.rot += localOffset;

		m_TransformChanged |= TransformChanged::ROTATION;
	}
}

const rujin::Scale& rujin::TransformComponent::GetScale() const
{
	return m_GlobalTransform.scale;
}

void rujin::TransformComponent::SetScale(const Scale& scale)
{
	if (const Scale localOffset = scale - GetScale(); abs(scale.x) + abs(scale.y) > 2 * FLT_EPSILON )
	{
		m_LocalTransform.scale += localOffset;

		m_TransformChanged |= TransformChanged::SCALE;
	}
}

const rujin::Transform& rujin::TransformComponent::GetTransform() const
{
	return m_GlobalTransform;
}

const rujin::Position& rujin::TransformComponent::GetLocalPosition() const
{
	return m_LocalTransform.pos;
}

void rujin::TransformComponent::SetLocalPosition(const Position& pos)
{
	m_LocalTransform.pos = pos;

	m_TransformChanged |= TransformChanged::POSITION;
}

void rujin::TransformComponent::AddLocalPosition(const Position& pos)
{
	if (abs(pos.x) + abs(pos.y) > 2 * FLT_EPSILON)
	{
		m_LocalTransform.pos += pos;

		m_TransformChanged |= TransformChanged::POSITION;
	}
}

rujin::Rotation rujin::TransformComponent::GetLocalRotation() const
{
	return m_LocalTransform.rot;
}

void rujin::TransformComponent::SetLocalRotation(const Rotation rot)
{
	m_LocalTransform.rot = fmod(rot, std::numbers::pi_v<float> *2.f);

	m_TransformChanged |= TransformChanged::ROTATION;
}
void rujin::TransformComponent::AddLocalRotation(Rotation rot)
{
	if (abs(rot) > FLT_EPSILON)
	{
		m_LocalTransform.rot = fmod(m_LocalTransform.rot + rot, std::numbers::pi_v<float> *2.f);

		m_TransformChanged |= TransformChanged::ROTATION;
	}
}

const rujin::Scale& rujin::TransformComponent::GetLocalScale() const
{
	return m_LocalTransform.scale;
}

void rujin::TransformComponent::SetLocalScale(const Scale& scale)
{
	m_LocalTransform.scale = scale;

	m_TransformChanged |= TransformChanged::SCALE;
}
void rujin::TransformComponent::AddLocalScale(const Scale& scale)
{
	if (abs(scale.x) + abs(scale.y) > 2 * FLT_EPSILON)
	{
		m_LocalTransform.scale += scale;

		m_TransformChanged |= TransformChanged::SCALE;
	}
}

const rujin::Transform& rujin::TransformComponent::GetLocalTransform() const
{
	return m_LocalTransform;
}

void rujin::TransformComponent::Start()
{
	//we want to update at start once for the construction transformations to go through
	Update();
}

void rujin::TransformComponent::Update()
{
	if (static_cast<uint32_t>(m_TransformChanged) > 0)
	{
		UpdateSelfAndChildren();

		//notify changes
		const event::OnTransformChanged_t event(this, m_TransformChanged);
		Notify(static_cast<uint32_t>(event::Identifier::OnTransformChanged), &event);

		m_TransformChanged = {};
	}
}

void rujin::TransformComponent::UpdateSelfAndChildren()
{
	if (const rujin::GameObject* pParent = GameObject()->GetParent(); pParent)
		m_LocalTransform.worldMatrix = pParent->GetTransform()->GetLocalTransform().worldMatrix * m_LocalTransform.GetLocalModelMatrix();
	else
		m_LocalTransform.worldMatrix = m_LocalTransform.GetLocalModelMatrix();

	//decompose and update global transform
	rutils::Decompose(m_LocalTransform.worldMatrix, m_GlobalTransform.pos, m_GlobalTransform.rot, m_GlobalTransform.scale);

	//update children.
	for (const std::unique_ptr<rujin::GameObject>& child : GameObject()->GetChildren())
		child->GetTransform()->UpdateSelfAndChildren();
}
