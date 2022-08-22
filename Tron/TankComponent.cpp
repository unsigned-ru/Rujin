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
#include "TronMovementComponent.h"
#include "TextureRenderComponent.h"


TankComponent::TankComponent
(
	TronMovementComponent* pTankMovement,
	TankAimingComponent* pTankAiming, 
	TextureRenderComponent* pTankBodyRenderer,
	TextureRenderComponent* pTankTurretRenderer,
	BoxColliderComponent* pTankCollider,
	HealthComponent* pHealth,
	const Recti& bulletSourceRect,
	float bulletSpeed,
	uint32_t bulletDamage,
	uint32_t shootingCooldown,
	uint8_t bulletBounces
)
	: m_pMovement(pTankMovement)
	, m_pAiming(pTankAiming)
	, m_pBodyRenderer(pTankBodyRenderer)
	, m_pTurretRenderer(pTankTurretRenderer)
	, m_pBodyCollider(pTankCollider)
	, m_pHealth(pHealth)
	, m_BulletSrcRect(bulletSourceRect)
	, m_BulletBounces(bulletBounces)
	, m_BulletSpeed(bulletSpeed)
	, m_BulletDamage(bulletDamage)
	, m_ShootingCooldown(shootingCooldown)
{

	ASSERT_MSG(m_pMovement, "This component requires a TronMovementComponent.");
	ASSERT_MSG(m_pAiming, "This component requires a TankAimingComponent.");
	ASSERT_MSG(m_pBodyRenderer, "This component requires a TextureRenderComponent for the body.");
	ASSERT_MSG(m_pBodyCollider, "This component requires a BoxColliderComponent.");
}

TronMovementComponent* TankComponent::GetMovement() const { return m_pMovement;}
TankAimingComponent* TankComponent::GetAiming() const { return m_pAiming;}
TextureRenderComponent* TankComponent::GetBodyRenderer() const { return m_pBodyRenderer;}
TextureRenderComponent* TankComponent::GetTurretRenderer() const { return m_pTurretRenderer;}
BoxColliderComponent* TankComponent::GetColliderComponent() const { return m_pBodyCollider; }
HealthComponent* TankComponent::GetHealthComponent() const { return m_pHealth; }

bool TankComponent::CanShoot() const
{
	return std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - m_LastShotTime).count() > m_ShootingCooldown;
}

void TankComponent::Shoot()
{
	//check if we can shoot yet
	if (!CanShoot())
		return;

	//get bullet spawn position and rotation from socket.
	Position bulletSpawnPos;
	Rotation bulletSpawnRot;
	glm::vec2 bulletDirection;
	m_pAiming->GetBulletSocket(bulletSpawnPos, bulletSpawnRot, bulletDirection);

	
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
	pBulletGO->AddComponent(new TextureRenderComponent(ServiceLocator::GetService<ResourceService>().GetTexture("Textures/Spritesheet.png"), {0.5f, 0.5f}, m_BulletSrcRect));
	pBulletGO->AddComponent(new TankBulletComponent(this, m_BulletBounces, m_BulletSpeed, m_BulletDamage));

	pBulletGO->GetTransform().SetPosition(bulletSpawnPos);
	pBulletGO->GetTransform().SetLocalRotation(bulletSpawnRot);

	GameObject()->GetScene()->AddGameObject(pBulletGO);

	if (GameObject()->HasTag("Faction1"))
		pBulletGO->AddTag("Faction1Bullet");
	else if (GameObject()->HasTag("Faction2"))
		pBulletGO->AddTag("Faction2Bullet");

	m_LastShotTime = std::chrono::high_resolution_clock::now();
}