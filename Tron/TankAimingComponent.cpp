#include "TronPCH.h"
#include "TankAimingComponent.h"

#include "GameObject.h"
#include "RenderService.h"
#include "ServiceLocator.h"
#include "Rutils/Math.h"

void TankAimingComponent::AimAt(const glm::vec2& direction)
{
	//TODO: fix bug
	Transform& transform = GameObject()->GetTransform();

	//get the angle required to face the direction
	const float angleRequired = rutils::GetAngleBetween(GetSocketOffset(), direction);

	//rotate GO so we face the direction
	transform.AddLocalRotation(angleRequired);
}

glm::vec2 TankAimingComponent::GetSocketOffset() const
{
	const Transform& transform = GameObject()->GetTransform();
	const Rotation rotation = transform.GetRotation();
	const glm::vec2 socketVector{ s_BulletSpawnDistance, 0.f };

	return rutils::Rotate(socketVector, rotation);
}

void TankAimingComponent::Draw() const
{
	//TODO: remove
	const RenderService& renderer = ServiceLocator::GetService<RenderService>();
	const Transform& pTransform = GameObject()->GetTransform();

	renderer.DrawPoint(pTransform.GetPosition(), 5.f);

}
