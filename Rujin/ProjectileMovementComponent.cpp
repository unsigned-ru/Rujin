#include "RujinPCH.h"
#include "ProjectileMovementComponent.h"

#include "GameObject.h"
#include "Rujin.h"

rujin::ProjectileMovementComponent::ProjectileMovementComponent(const glm::vec2& initialVelocity)
	: m_Velocity(initialVelocity)
{}

void rujin::ProjectileMovementComponent::FixedUpdate()
{
	GameObject()->GetTransform().AddLocalPosition(Rujin::Get()->GetFixedUpdateDeltaTime() * m_Velocity);
}

void rujin::ProjectileMovementComponent::SetVelocity(const glm::vec2& velocity)
{
	m_Velocity = velocity;
}

const glm::vec2& rujin::ProjectileMovementComponent::GetVelocity() const
{
	return m_Velocity;
}
