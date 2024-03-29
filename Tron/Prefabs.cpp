#include "TronPCH.h"
#include "Prefabs.h"

#include "BoxColliderComponent.h"
#include "GameObject.h"
#include "GridTextureRenderComponent.h"
#include "HealthComponent.h"
#include "PlayerHUDComponent.h"
#include "RandomTeleporterComponent.h"
#include "RecognizerAIController.h"
#include "RecognizerComponent.h"
#include "ResourceService.h"
#include "ServiceLocator.h"
#include "TankAIController.h"
#include "TankAimingComponent.h"
#include "TankComponent.h"
#include "TextRenderComponent.h"
#include "TextureRenderComponent.h"
#include "TronMovementComponent.h"
#include "TronPlayerComponent.h"

GameObject* prefabs::CreatePlayerTank(const std::string& name, rujin::PlayerIndex playerIdx)
{
	auto& resources = ServiceLocator::GetService<ResourceService>();
	GameObject* playerGO = new GameObject(name);

	auto* pBodyRenderer = playerGO->AddComponent
	(
		new TextureRenderComponent
		(
			resources.GetTexture("Textures/Spritesheet.png"),
			{ 0.5f, 0.5f },
			Recti{ (playerIdx % 2) * 50, 0, 50, 50}
		)
	);

	auto* pTankCollider = playerGO->AddComponent
	(
		new BoxColliderComponent({ 50, 50 }, false)
	);

	auto* pTankMovement = playerGO->AddComponent
	(
		new TronMovementComponent(pTankCollider, 100.f)
	);

#ifdef _DEBUG
	playerGO->GetComponent<BoxColliderComponent>()->EnableDebugDrawing();
#endif //_DEBUG

	/* Turret Start */
	GameObject* Turret = new GameObject("Turret");

	auto* pTankAimingComponent = Turret->AddComponent
	(
		new TankAimingComponent()
	);

	auto* pTurretRenderer = Turret->AddComponent
	(
		new TextureRenderComponent
		(
			resources.GetTexture("Textures/Spritesheet.png"),
			{ 0.2f, 0.5f },
			Recti{ 0, 50 + (playerIdx % 2) * 21, 45, 21 }
		)
	);

	Turret->GetTransform().AddLocalPosition({ -8.f, 0.f });
	playerGO->AddChild(Turret);
	/* Turret End */

	auto* pHealth = playerGO->AddComponent(new HealthComponent(1));

	auto* pTank = playerGO->AddComponent
	(
		new TankComponent
		(
			pTankMovement,
			pTankAimingComponent,
			pBodyRenderer,
			pTurretRenderer,
			pTankCollider,
			pHealth,
			Recti{ 50, 50 + (playerIdx % 2) * 19, 19, 14 }
		)
	);

	
	playerGO->AddComponent(new TronPlayerComponent(pTank, playerIdx));

	return playerGO;
}

GameObject* prefabs::CreateEnemyTank()
{
	auto& resources = ServiceLocator::GetService<ResourceService>();
	GameObject* pEnemyGO = new GameObject("Enemy_Tank");

	auto* pBodyRenderer = pEnemyGO->AddComponent
	(
		new TextureRenderComponent
		(
			resources.GetTexture("Textures/Spritesheet.png"),
			{ 0.5f, 0.5f },
			Recti{ 100, 0, 50, 50 }
		)
	);

	auto* pTankCollider = pEnemyGO->AddComponent
	(
		new BoxColliderComponent({ 50, 50 }, false)
	);

	auto* pTankMovement = pEnemyGO->AddComponent
	(
		new TronMovementComponent(pTankCollider, 75.f)
	);

#ifdef _DEBUG
	pEnemyGO->GetComponent<BoxColliderComponent>()->EnableDebugDrawing();
#endif //_DEBUG

	auto* pTankAimingComponent = pEnemyGO->AddComponent
	(
		new TankAimingComponent(45.f)
	);

	auto* pHealth = pEnemyGO->AddComponent(new HealthComponent(3));
	auto* pTank = pEnemyGO->AddComponent
	(
		new TankComponent
		(
			pTankMovement,
			pTankAimingComponent,
			pBodyRenderer,
			nullptr,
			pTankCollider,
			pHealth,
			Recti{ 50, 50 + 2 * 15, 19, 15 },
			700, 1, 500, 0
		)
	);
	
	pEnemyGO->AddComponent(new TankAIController(pTank));

	return pEnemyGO;
}

GameObject* prefabs::CreateEnemyRecognizer()
{
	auto& resources = ServiceLocator::GetService<ResourceService>();
	GameObject* pEnemyGO = new GameObject("Enemy_Recognizer");

	auto* pBodyRenderer = pEnemyGO->AddComponent
	(
		new TextureRenderComponent
		(
			resources.GetTexture("Textures/Spritesheet.png"),
			{ 0.5f, 0.5f },
			Recti{ 150, 0, 50, 50 }
		)
	);

	auto* pCollider = pEnemyGO->AddComponent
	(
		new BoxColliderComponent({ 50, 50 }, false)
	);

	auto* pTankMovement = pEnemyGO->AddComponent
	(
		new TronMovementComponent(pCollider, 100)
	);

#ifdef _DEBUG
	pEnemyGO->GetComponent<BoxColliderComponent>()->EnableDebugDrawing();
#endif //_DEBUG

	auto* pHealth = pEnemyGO->AddComponent(new HealthComponent(3));

	auto* pRecognizer = pEnemyGO->AddComponent
	(
		new RecognizerComponent
		(
			pTankMovement,
			pBodyRenderer,
			pCollider,
			pHealth
		)
	);

	pEnemyGO->AddComponent(new RecognizerAIController(pRecognizer));
	

	return pEnemyGO;
}

GameObject* prefabs::CreatePlayerHUD(const std::string& name, TronPlayerComponent* pPlayer, float horizontalOffset, const glm::vec2& pivot)
{
	auto& resources = ServiceLocator::GetService<ResourceService>();

	GameObject* pPlayerHUD = new GameObject(name);
	pPlayerHUD->AddTag("LevelPersistent");

	GameObject* pLivesChild = new GameObject(name + "_Lives");
	auto* pLivesRenderer = pLivesChild->AddComponent
	(
		new GridTextureRenderComponent
		(
			glm::ivec2{ 3, 1 },
			glm::vec2{ 10.f, 0.f },
			resources.GetTexture("Textures/Spritesheet.png"),
			pivot,
			Recti{0, 150, 38, 50},
			abs(pivot.x - 1.f) <= FLT_EPSILON
		)
	);

	pLivesChild->GetTransform().AddLocalPosition({ horizontalOffset, -10.f });

	GameObject* pScoreChild = new GameObject(name + "_Score");
	auto* pScoreRenderer = pScoreChild->AddComponent
	(
		new TextRenderComponent
		(
			resources.GetFont("Fonts/Arcade.ttf", 50),
			glm::vec4{1.f, 1.f, 1.f, 1.f},
			pivot
		)
	);
	pScoreChild->GetTransform().AddLocalPosition({ horizontalOffset, -60.f });

	pPlayerHUD->AddChild(pLivesChild);
	pPlayerHUD->AddChild(pScoreChild);

	pPlayerHUD->AddComponent
	(
		new PlayerHUDComponent
		(
			pPlayer,
			pLivesRenderer,
			pScoreRenderer
		)
	);


	return pPlayerHUD;
}

GameObject* prefabs::CreateRandomTeleporter(const std::string& name)
{
	auto& resources = ServiceLocator::GetService<ResourceService>();

	GameObject* pRandomTeleporterGO = new GameObject(name);
	pRandomTeleporterGO->AddComponent
	(
		new TextureRenderComponent
		(
			resources.GetTexture("Textures/Spritesheet.png"),
			{ 0.5f, 0.5f },
			Recti{ 0, 200, 100, 100 }
		)
	);

	pRandomTeleporterGO->AddComponent
	(
		new BoxColliderComponent
		(
			{75, 75},
			true,
			{0.5f, 0.5f},
			CollisionLayer::OverlapAll
		)
	);

	pRandomTeleporterGO->AddComponent(new RandomTeleporterComponent());

	return pRandomTeleporterGO;
}
