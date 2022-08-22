#include "TronPCH.h"
#include "TankBulletComponent.h"

#include "Collider.h"
#include "ColliderComponent.h"
#include "GameEvents.h"
#include "GameObject.h"
#include "HealthComponent.h"
#include "ProjectileMovementComponent.h"
#include "TankComponent.h"

TankBulletComponent::TankBulletComponent(TankComponent* pOwner, uint8_t maxBounces, float bulletSpeed, uint32_t damage)
	: m_MaxBounces(maxBounces)
	, m_BulletSpeed(bulletSpeed)
	, m_Damage(damage)
	, m_pOwnerTank(pOwner)
{
}


void TankBulletComponent::Start()
{
	m_pProjectileMovement = GameObject()->GetComponent<ProjectileMovementComponent>();
	ASSERT_MSG(m_pProjectileMovement, "This component requires a ProjectileMovementComponent to be present on the GameObject.");
	ASSERT_MSG(GameObject()->GetComponent<BoxColliderComponent>(), "This component requires a BoxColliderComponent to be present on the GameObject.");

	GameObject()->AddObserver(this);
	m_pOwnerTank->GetHealthComponent()->AddObserver(this);
}

void TankBulletComponent::FixedUpdate()
{
	Component::FixedUpdate();

	m_HitEnemyThisFrame = false;
	m_HitWallThisFrame = false;
}

void TankBulletComponent::OnOverlap(const CollisionResult& result)
{
	const class GameObject* pGameObject = result.other->GetComponent()->GameObject();


	if (!m_pOwnerTank)
		return;

	//hitting a wall...
	if (!m_HitWallThisFrame && pGameObject->HasTag("Wall"))
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

		m_HitWallThisFrame = true;

		return;
	}

	if (!m_HitEnemyThisFrame)
	{
		if (GameObject()->HasTag("Faction1Bullet") && pGameObject->HasTag("Faction2"))
		{
			//player bullet hit enemy.
			if (HealthComponent* pHealth = pGameObject->GetComponent<HealthComponent>(); pHealth)
			{
				pHealth->TakeDamage(m_Damage, m_pOwnerTank->GameObject());
			}

			m_HitEnemyThisFrame = true;

			m_pGameObject->Destroy();
		}
		else if (GameObject()->HasTag("Faction2Bullet") && pGameObject->HasTag("Faction1"))
		{
			//Enemy bullet hit Player
			if (HealthComponent* pHealth = pGameObject->GetComponent<HealthComponent>(); pHealth)
			{
				pHealth->TakeDamage(m_Damage, m_pOwnerTank->GameObject());
			}

			m_HitEnemyThisFrame = true;

			m_pGameObject->Destroy();
		}
	}
	
}

void TankBulletComponent::OnNotify(const uint32_t identifier, const event::Data* pEventData)
{
	if (identifier == static_cast<uint32_t>(game_event::Identifier::OnDie))
	{
		const auto* pOnDieEvent = static_cast<const game_event::OnDie_t*>(pEventData);

		if (pOnDieEvent->pHealth->GameObject() == m_pOwnerTank->GameObject())
		{
			pOnDieEvent->pHealth->RemoveObserver(this);
			GameObject()->GetRootParent()->Destroy();

			m_pOwnerTank = nullptr;
		}
	}
	else if (identifier == static_cast<uint32_t>(event::Identifier::OnGameObjectDestroyed))
	{
		const auto* pOnDestroyedEvent = static_cast<const event::OnGameObjectDestroyed_t*>(pEventData);

		if (pOnDestroyedEvent->pObject == GameObject() && m_pOwnerTank)
		{
			m_pOwnerTank->GetHealthComponent()->RemoveObserver(this);
		}

	}
}
