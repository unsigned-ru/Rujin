#include "TronPCH.h"
#include "TankAimingComponent.h"

#include "GameObject.h"
#include "RenderService.h"
#include "ServiceLocator.h"
#include "Rutils/Math.h"

void TankAimingComponent::AimAt(const glm::vec2& direction)
{
	Transform& transform = GameObject()->GetTransform();

	//get the angle required to face the direction
	const float angleRequired = atan2f(direction.y, direction.x);

	//rotate GO so we face the direction
	transform.SetRotation(angleRequired);
}

void TankAimingComponent::GetBulletSocket(Position& pos, Rotation& rot, glm::vec2& barrelDirection) const
{
	const Transform& transform = m_pGameObject->GetTransform();
	const glm::vec2 socketOffset = rutils::Rotate({ s_BulletSpawnDistance, 0.f }, transform.GetRotation());

	pos = transform.GetPosition() + socketOffset;
	rot = atan2f(socketOffset.y, socketOffset.x);
	barrelDirection = normalize(socketOffset);
}
