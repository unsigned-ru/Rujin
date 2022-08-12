#include "TronPCH.h"
#include "TankComponent.h"

#include "BoxColliderComponent.h"
#include "GameObject.h"
#include "ProjectileMovementComponent.h"
#include "ResourceService.h"
#include "Scene.h"
#include "ServiceLocator.h"
#include "TankAimingComponent.h"
#include "TankBulletComponent.h"
#include "TankMovementComponent.h"
#include "TextureRenderComponent.h"


TankComponent::TankComponent(float maxHealth, TankMovementComponent* pTankMovement, TankAimingComponent* pTankAiming, TextureRenderComponent* pTankBodyRenderer, TextureRenderComponent* pTankTurretRenderer, BoxColliderComponent* pTankCollider)
	: m_pTankMovement(pTankMovement)
	, m_pTankAiming(pTankAiming)
	, m_pTankBodyRenderer(pTankBodyRenderer)
	, m_pTankTurretRenderer(pTankTurretRenderer)
	, m_pBoxCollider(pTankCollider)
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

	rujin::GameObject* pBulletGO = new rujin::GameObject(GameObject()->GetName() + "_Bullet");
	pBulletGO->AddComponent(new BoxColliderComponent({ 15, 15 }, false, true))->EnableDebugDrawing();
 	pBulletGO->AddComponent(new ProjectileMovementComponent(bulletDirection * m_BulletSpeed));
	pBulletGO->AddComponent(new TextureRenderComponent(ServiceLocator::GetService<ResourceService>().LoadTexture("Textures/Spritesheet.png"), {0.5f, 0.5f}, Recti{50, 50, 19, 15}));
	pBulletGO->AddComponent(new TankBulletComponent(m_MaxBounces, m_BulletSpeed,m_BulletDamage));

	pBulletGO->GetTransform().SetPosition(bulletSpawnPos);
	pBulletGO->GetTransform().SetLocalRotation(bulletSpawnRot);

	GameObject()->GetScene()->AddGameObject(pBulletGO);
}

void TankComponent::TakeDamage(float damage)
{
	m_CurrentHealth -= damage;
}

void TankComponent::OnOverlap(const CollisionResult&)
{
	LOG_DEBUG_("Overlapping");
}
