#include "TronPCH.h"
#include "TankComponent.h"

#include "BoxColliderComponent.h"
#include "CollisionFunctions.h"
#include "GameObject.h"
#include "ProjectileMovementComponent.h"
#include "ResourceService.h"
#include "Scene.h"
#include "ServiceLocator.h"
#include "TankAimingComponent.h"
#include "TankBulletComponent.h"
#include "TankMovementComponent.h"
#include "TextureRenderComponent.h"


TankComponent::TankComponent
(
	TankMovementComponent* pTankMovement,
	TankAimingComponent* pTankAiming, 
	TextureRenderComponent* pTankBodyRenderer,
	TextureRenderComponent* pTankTurretRenderer,
	BoxColliderComponent* pTankCollider,
	const Recti& bulletSourceRect,
	float maxHealth,
	float bulletSpeed,
	float bulletDamage,
	uint8_t bulletBounces
)
	: m_pTankMovement(pTankMovement)
	, m_pTankAiming(pTankAiming)
	, m_pTankBodyRenderer(pTankBodyRenderer)
	, m_pTankTurretRenderer(pTankTurretRenderer)
	, m_pBoxCollider(pTankCollider)
	, m_BulletSrcRect(bulletSourceRect)
	, m_BulletBounces(bulletBounces)
	, m_BulletSpeed(bulletSpeed)
	, m_BulletDamage(bulletDamage)
	, m_MaxHealth(maxHealth)
{

	ASSERT_MSG(m_pTankMovement, "This component requires a TankMovementComponent.");
	ASSERT_MSG(m_pTankAiming, "This component requires a TankAimingComponent.");
	ASSERT_MSG(m_pTankBodyRenderer, "This component requires a TextureRenderComponent for the body.");
	ASSERT_MSG(m_pBoxCollider, "This component requires a BoxColliderComponent.");

	m_pTankMovement->SetTank(this);
}

TankMovementComponent* TankComponent::GetMovement() const
{
	return m_pTankMovement;
}

TankAimingComponent* TankComponent::GetAiming() const
{
	return m_pTankAiming;
}

TextureRenderComponent* TankComponent::GetBodyRenderer() const
{
	return m_pTankBodyRenderer;
}

TextureRenderComponent* TankComponent::GetTurretRenderer() const
{
	return m_pTankTurretRenderer;
}

BoxColliderComponent* TankComponent::GetColliderComponent() const
{
	return m_pBoxCollider;
}

void TankComponent::Shoot()
{
	//get bullet spawn position and rotation from socket.
	Position bulletSpawnPos;
	Rotation bulletSpawnRot;
	glm::vec2 bulletDirection;
	m_pTankAiming->GetBulletSocket(bulletSpawnPos, bulletSpawnRot, bulletDirection);

	
	//create bullet rect
 	const Rectf bulletRect{ bulletSpawnPos, {15, 15} };

	//check if bullet will spawn inside wall
	for (const Collider* pPossibleOverlapCollider : GameObject()->GetScene()->GetCollisionQuadTree()->Search(bulletRect))
	{
		//if the response to the collider isn't block, then we don't need to worry about it
		if (GetResponseForCollisionLayer(pPossibleOverlapCollider->GetCollisionLayer(), CollisionLayer::Bullet) != CollisionResponse::Block)
			continue;

		if (const BoxCollider* pPossibleOverlapBoxCollider = dynamic_cast<const BoxCollider*>(pPossibleOverlapCollider); pPossibleOverlapBoxCollider)
		{
			
			const auto possibleOverlapBox = pPossibleOverlapBoxCollider->GetRect();
			if (collision::IsInside(bulletRect, possibleOverlapBox) || collision::IsOverlapping(bulletRect, possibleOverlapBox))
				return; // bullet will spawn in wall, don't spawn bullet.
		}
		else
			LOG_WARNING("Unimplemented Collider detected. Did you forget to implement?");
	}


	auto* pBulletGO = new rujin::GameObject(GameObject()->GetName() + "_Bullet");
	pBulletGO->AddComponent(new BoxColliderComponent({ 15, 15 }, false, glm::vec2{ 0.5f, 0.5f }, CollisionLayer::Bullet));
 	pBulletGO->AddComponent(new ProjectileMovementComponent(bulletDirection * m_BulletSpeed));
	pBulletGO->AddComponent(new TextureRenderComponent(ServiceLocator::GetService<ResourceService>().LoadTexture("Textures/Spritesheet.png"), {0.5f, 0.5f}, m_BulletSrcRect));
	pBulletGO->AddComponent(new TankBulletComponent(this, m_BulletBounces, m_BulletSpeed, m_BulletDamage));

	pBulletGO->GetTransform().SetPosition(bulletSpawnPos);
	pBulletGO->GetTransform().SetLocalRotation(bulletSpawnRot);

	GameObject()->GetScene()->AddGameObject(pBulletGO);
}

void TankComponent::TakeDamage(float damage)
{
	m_CurrentHealth -= damage;
}