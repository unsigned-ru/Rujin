#include "RujinPCH.h"

#include "GameObject.h"
#include "Transform.h"

#include "EngineEvents.h"

#include "Rutils/Math.h"

rujin::Transform::Transform(GameObject* pOwner)
	: Subject({})
	, m_pGameObject(pOwner)
{
}

const rujin::Position& rujin::Transform::GetPosition() const
{
	return m_GlobalPosition;
}

void rujin::Transform::SetPosition(const rujin::Position& pos)
{
	//localOffset for global = newGlobal - global
	if (const Position localOffset{ pos - GetPosition() }; abs(localOffset.x) + abs(localOffset.y) > 2 * FLT_EPSILON)
	{
		m_LocalPosition += localOffset;

		m_TransformChanged |= TransformChanged::POSITION;
	}
}

rujin::Rotation rujin::Transform::GetRotation() const
{
	return m_GlobalRotation;
}


void rujin::Transform::SetRotation(const Rotation rot)
{
	//calculate difference between requested global rotation and current global rotation
	//and add locally
	if (const Rotation localOffset = rot - GetRotation(); abs(localOffset) > FLT_EPSILON)
	{
		m_LocalRotation += localOffset;

		m_TransformChanged |= TransformChanged::ROTATION;
	}
}

const rujin::Scale& rujin::Transform::GetScale() const
{
	return m_GlobalScale;
}

void rujin::Transform::SetScale(const Scale& scale)
{
	if (const Scale localOffset = scale - GetScale(); abs(scale.x) + abs(scale.y) > 2 * FLT_EPSILON )
	{
		m_LocalScale += localOffset;

		m_TransformChanged |= TransformChanged::SCALE;
	}
}

const rujin::Position& rujin::Transform::GetLocalPosition() const
{
	return m_LocalPosition;
}

void rujin::Transform::SetLocalPosition(const Position& pos)
{
	m_LocalPosition = pos;

	m_TransformChanged |= TransformChanged::POSITION;
}

void rujin::Transform::AddLocalPosition(const Position& pos)
{
	if (abs(pos.x) + abs(pos.y) > 2 * FLT_EPSILON)
	{
		m_LocalPosition += pos;

		m_TransformChanged |= TransformChanged::POSITION;
	}
}

rujin::Rotation rujin::Transform::GetLocalRotation() const
{
	return m_LocalRotation;
}

void rujin::Transform::SetLocalRotation(const Rotation rot)
{
	m_LocalRotation = fmod(rot, std::numbers::pi_v<float> *2.f);

	m_TransformChanged |= TransformChanged::ROTATION;
}
void rujin::Transform::AddLocalRotation(Rotation rot)
{
	if (abs(rot) > FLT_EPSILON)
	{
		m_LocalRotation = fmod(m_LocalRotation + rot, std::numbers::pi_v<float> *2.f);

		m_TransformChanged |= TransformChanged::ROTATION;
	}
}

const rujin::Scale& rujin::Transform::GetLocalScale() const
{
	return m_LocalScale;
}

void rujin::Transform::SetLocalScale(const Scale& scale)
{
	m_LocalScale = scale;

	m_TransformChanged |= TransformChanged::SCALE;
}

void rujin::Transform::AddLocalScale(const Scale& scale)
{
	if (abs(scale.x) + abs(scale.y) > 2 * FLT_EPSILON)
	{
		m_LocalScale += scale;

		m_TransformChanged |= TransformChanged::SCALE;
	}
}

rujin::GameObject* rujin::Transform::GetGameObject() const
{
	return m_pGameObject;
}

void rujin::Transform::UpdateSelfAndChildren()
{
	if (static_cast<uint32_t>(m_TransformChanged) > 0) //if transform has changed
		ApplyTransformationsToSelfAndChildren();
	else
		//if transform hasn't changed, check if child transform might have changed...
		for (const std::unique_ptr<GameObject>& child : m_pGameObject->GetChildren().GetVector())
			child->GetTransform().UpdateSelfAndChildren();
}

void rujin::Transform::ApplyTransformationsToSelfAndChildren()
{
	if (GameObject* pParent = m_pGameObject->GetParent(); pParent)
		m_WorldMatrix = pParent->GetTransform().m_WorldMatrix * GetLocalModelMatrix();
	else
		m_WorldMatrix = GetLocalModelMatrix();

	//decompose and update global transform
	rutils::Decompose(m_WorldMatrix, m_GlobalPosition, m_GlobalRotation, m_GlobalScale);

	//update children.
	for (const std::unique_ptr<GameObject>& child : m_pGameObject->GetChildren().GetVector())
		child->GetTransform().ApplyTransformationsToSelfAndChildren();

	//notify changes
	const event::OnTransformChanged_t event(*this, m_TransformChanged);
	Notify(static_cast<uint32_t>(event::Identifier::OnTransformChanged), &event);

	m_TransformChanged = {};
}

glm::mat3 rujin::Transform::GetLocalModelMatrix()
{
	const glm::mat3 translationMatrix
	{
		1.f,	0.f,	0.f,
		0.f,	1.f,	0.f,
		m_LocalPosition.x,	m_LocalPosition.y,	1.f
	};

	const glm::mat3 rotationMatrix
	{
		cos(m_LocalRotation),	sin(m_LocalRotation),	0,
		-sin(m_LocalRotation),	cos(m_LocalRotation),	0,
		0.f,		0.f,		1.f
	};

	const glm::mat3 scaleMatrix
	{
		m_LocalScale.x,	0.f,		0.f,
		0.f,		m_LocalScale.y,	0.f,
		0.f,		0.f,		1.f
	};

	return translationMatrix * rotationMatrix * scaleMatrix;
}
