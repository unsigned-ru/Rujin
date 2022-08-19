#include "TronPCH.h"
#include "TankBulletComponent.h"

#include "Collider.h"
#include "ColliderComponent.h"
#include "GameObject.h"
#include "HealthComponent.h"
#include "ProjectileMovementComponent.h"
#include "TankComponent.h"

TankBulletComponent::TankBulletComponent(TankComponent* pOwner, uint8_t maxBounces, float bulletSpeed, uint32_t damage)
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
	Component::FixedUpdate();
	m_HasHitPlayerThisFrame = false;
}

void TankBulletComponent::OnOverlap(const CollisionResult& result)
{
	const class GameObject* pGameObject = result.other->GetComponent()->GameObject();

	//hitting a wall...
	if (pGameObject->HasTag("Wall"))
	{
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
			m_pProjectileMovement->SetVelocity({ velocity.x, velocity.y * -1.f });
			break;
		}

		if (++m_CurrentBounces > m_MaxBounces)
		{
			m_pGameObject->Destroy();
		}

		return;
	}

	if (m_pOwningTank->GameObject()->HasTag("Player") && pGameObject->HasTag("Enemy"))
	{
		if (m_HasHitPlayerThisFrame)
			return;

		m_HasHitPlayerThisFrame = true; //TODO: clean up
		//player bullet hit enemy.
		if (HealthComponent* pHealth = pGameObject->GetComponent<HealthComponent>(); pHealth)
		{
			pHealth->TakeDamage(m_Damage, m_pOwningTank->GameObject());
		}

		m_pGameObject->Destroy();
	}

	else if(m_pOwningTank->GameObject()->HasTag("Enemy") && pGameObject->HasTag("Player"))
	{
		if (m_HasHitPlayerThisFrame)
			return;

		m_HasHitPlayerThisFrame = true; //TODO: clean up
		//Enemy bullet hit Player
		if (HealthComponent* pHealth = pGameObject->GetComponent<HealthComponent>(); pHealth)
		{
			pHealth->TakeDamage(m_Damage, m_pOwningTank->GameObject());
		}

		m_pGameObject->Destroy();
	}
}
