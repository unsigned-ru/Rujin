#include "TronPCH.h"
#include "TankBulletComponent.h"

#include "Collider.h"
#include "ColliderComponent.h"
#include "GameObject.h"
#include "ProjectileMovementComponent.h"
#include "TankComponent.h"

TankBulletComponent::TankBulletComponent(TankComponent* pOwner, uint8_t maxBounces, float bulletSpeed, float damage)
	: m_MaxBounces(maxBounces)
	, m_BulletSpeed(bulletSpeed)
	, m_Damage(damage)
	, m_pOwningTank(pOwner)
{
}

void TankBulletComponent::Start()
{
	m_pProjectileMovement = GameObject()->GetComponent<ProjectileMovementComponent>();
	ASSERT_MSG(m_pProjectileMovement, "This component requires a ProjectileMovementComponent to be present on the GameObject.");
	ASSERT_MSG(GameObject()->GetComponent<BoxColliderComponent>(), "This component requires a BoxColliderComponent to be present on the GameObject.");
}

void TankBulletComponent::FixedUpdate()
{
	m_HitWallThisFrame = false;
}

void TankBulletComponent::OnOverlap(const CollisionResult& result)
{
	
	if (TankComponent* pTank = result.other->GetComponent()->GameObject()->GetComponent<TankComponent>())
	{
		//we hit a tank
		pTank->TakeDamage(m_Damage);

		//remove gameobject
		m_pGameObject->Destroy();

	}
	else
	{
		if (m_HitWallThisFrame)
			return;

		const glm::vec2& velocity = m_pProjectileMovement->GetVelocity();
		//we hit something else.
		switch (result.collisionDirection)
		{
		case Direction::LEFT: //intentional fall-through
		case Direction::RIGHT:
			m_pProjectileMovement->SetVelocity({ velocity.x * -1.f, velocity.y });
			break;
		case Direction::UP: //intentional fall-through
		case Direction::DOWN:
			m_pProjectileMovement->SetVelocity({ velocity.x, velocity.y * -1.f});
			break;
		}

		m_HitWallThisFrame = true;

		if (++m_CurrentBounces > m_MaxBounces)
		{
			m_pGameObject->Destroy();
		}
	}
}
