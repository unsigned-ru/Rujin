﻿#include "TronPCH.h"
#include "Tron.h"

#include <SDL_image.h>

#include "BoxColliderComponent.h"
#include "FMOD_AudioProvider.h"
#include "GameObject.h"
#include "HeuristicFunctions.h"
#include "RecognizerAIController.h"
#include "RecognizerComponent.h"
#include "ResourceProvider.h"
#include "Rujin.h"
#include "Scene.h"
#include "SceneProvider.h"
#include "ServiceLocator.h"
#include "TankAimingComponent.h"
#include "TankComponent.h"
#include "TextureRenderComponent.h"
#include "TronPlayerComponent.h"
#include "TronMovementComponent.h"
#include "TankAIController.h"

using namespace rujin;

Tron::~Tron()
{
	delete m_pGridGraph;
}

void Tron::Configure(settings::InitParams& params)
{
	params.windowTitle = "Rujin Demo: Tron";
	params.windowSize = { 750 , 790 };
}

void Tron::Load()
{
	//AudioService& audioService = ServiceLocator::GetService<AudioService>();
	ResourceService& resourceService = ServiceLocator::GetService<ResourceService>();
	SceneService& sceneService = ServiceLocator::GetService<SceneService>();

	/* Load required audio...*/
	//...

	/* Create scene with collision field with the size of the screen. */
	const auto& windowSize = Rujin::Get()->GetWindowContext().windowSize;
	Scene* pScene = sceneService.CreateScene("Tron Demo Scene", Rectf{0, 0, static_cast<float>(windowSize.x), static_cast<float>(windowSize.y)});

	///* Create Circuit background */
	{
		GameObject* go = new GameObject("Circuit_BG");
		go->AddComponent(new TextureRenderComponent(resourceService.LoadTexture("Textures/Circuit.png"), {0.f, 0.f}));
		pScene->AddGameObject(go);
	}

	//create pathfinding 
	m_pGridGraph = new graph::GridGraph<graph::GridTerrainNode, graph::GraphConnection>(m_GridStart, m_GridDimensions.x, m_GridDimensions.y, m_CellSize, false, false);

	/* Generate Level Grid */
	GenerateLevelGridFromTexture(pScene, "../Data/Levels/Level1.png");

	//Set pathfinding based on grid
	m_pAStar = new AStar(m_pGridGraph, heuristic_functions::Chebyshev);
	pScene->SetPathfinder(m_pAStar);

	/* Create surrounding colliders (so player can't move outside of bounds...*/
	CreateLevelBoundsColliders(pScene);

	/* Create player 1*/
	{
		GameObject* playerGO = new GameObject("Player1");

		auto* pBodyRenderer = playerGO->AddComponent
		(
			new TextureRenderComponent
			(
				resourceService.LoadTexture("Textures/Spritesheet.png"), 
				{0.5f, 0.5f},
				Recti{0, 0, 50, 50}
			)
		);

		auto* pTankCollider = playerGO->AddComponent
		(
			new BoxColliderComponent({ 50, 50 }, false)
		);

		auto* pTankMovement = playerGO->AddComponent
		(
			new TronMovementComponent(pTankCollider)
		);

#ifdef _DEBUG
		playerGO->GetComponent<BoxColliderComponent>()->EnableDebugDrawing();
#endif //_DEBUG

		//calc position
		playerGO->GetTransform().SetPosition
		(
			glm::vec2
			{
				(m_GridStart.x + m_CellSize),
				(m_GridStart.y + m_CellSize) 
			}
		);

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
				resourceService.LoadTexture("Textures/Spritesheet.png"), 
				{0.2f, 0.5f}, 
				Recti{0, 50, 45, 21}
			)
		);

		Turret->GetTransform().AddLocalPosition({ -8.f, 0.f });
		playerGO->AddChild(Turret);
		/* Turret End */

		auto* pTank = playerGO->AddComponent
		(
			new TankComponent
			(
				pTankMovement, 
				pTankAimingComponent, 
				pBodyRenderer, 
				pTurretRenderer, 
				pTankCollider,
				Recti{ 50, 50, 19, 14 }
			)
		);

		playerGO->AddComponent(new TronPlayerComponent(pTank));

		pScene->AddGameObject(playerGO);
	}


	/* Create Enemy Tank*/
	{
		GameObject* pEnemyGO = new GameObject("Enemy_Tank");

		auto* pBodyRenderer = pEnemyGO->AddComponent
		(
			new TextureRenderComponent
			(
				resourceService.LoadTexture("Textures/Spritesheet.png"),
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
			new TronMovementComponent(pTankCollider)
		);

#ifdef _DEBUG
		pEnemyGO->GetComponent<BoxColliderComponent>()->EnableDebugDrawing();
#endif //_DEBUG

		//calc position
		pEnemyGO->GetTransform().SetPosition
		(
			glm::vec2
			{
				(m_GridStart.x + m_GridDimensions.x * m_CellSize) / 2.f,
				(m_GridStart.y + m_GridDimensions.y * m_CellSize) / 2.f
			}
		);

		auto* pTankAimingComponent = pEnemyGO->AddComponent
		(
			new TankAimingComponent(45.f)
		);

		auto* pTank = pEnemyGO->AddComponent
		(
			new TankComponent
			(
				pTankMovement,
				pTankAimingComponent,
				pBodyRenderer,
				nullptr,
				pTankCollider,
				Recti{ 50, 50 + 2 * 15, 19, 15 }
			)
		);

		pEnemyGO->AddComponent(new TankAIController(pTank));

		pScene->AddGameObject(pEnemyGO);
	}

	/* Create Enemy Recognizer*/
	{
		GameObject* pEnemyGO = new GameObject("Enemy_Recognizer");

		auto* pBodyRenderer = pEnemyGO->AddComponent
		(
			new TextureRenderComponent
			(
				resourceService.LoadTexture("Textures/Spritesheet.png"),
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
			new TronMovementComponent(pCollider)
		);

#ifdef _DEBUG
		pEnemyGO->GetComponent<BoxColliderComponent>()->EnableDebugDrawing();
#endif //_DEBUG

		//calc position
		pEnemyGO->GetTransform().SetPosition
		(
			glm::vec2
			{
				(m_GridStart.x + m_GridDimensions.x * m_CellSize) - m_CellSize,
				(m_GridStart.y + m_GridDimensions.y * m_CellSize) - m_CellSize
			}
		);

		auto* pRecognizer = pEnemyGO->AddComponent
		(
			new RecognizerComponent
			(
				pTankMovement,
				pBodyRenderer,
				pCollider
			)
		);

		pEnemyGO->AddComponent(new RecognizerAIController(pRecognizer));

		pScene->AddGameObject(pEnemyGO);
	}
}

void Tron::GenerateLevelGridFromTexture(Scene* pScene, const std::string& levelTexturePath)
{
	/* Load level texture */
	SDL_Surface* pLevelTex = IMG_Load(levelTexturePath.c_str());

	if (!pLevelTex)
	{
		LOG_ERROR_("Error trying to load level layout file: {}", SDL_GetError());
		return; //to silence warning ... /facepalm
	}

	if (pLevelTex->w * pLevelTex->h != m_GridDimensions.x * m_GridDimensions.y)
		LOG_ERROR_("Level texture size wrong. Level texture must be {}x{}px!", m_GridDimensions.x, m_GridDimensions.y);

	ResourceService& resources = ServiceLocator::GetService<ResourceService>();

	const Recti floorSourceRect{ 100, 50, 25, 25 };

	//loop over the grid and create GameObjects.
	for (uint8_t y = 0; y < m_GridDimensions.y; ++y)
	{
		for (uint8_t x = 0; x < m_GridDimensions.x; ++x)
		{
			//calc position of cell.
			const Position pos{ m_GridStart.x + x * m_CellSize, m_GridStart.y + y * m_CellSize };

			//calculated red color of target pixel (ptr arithmetic: +1 = green, +1 = blue)  
			const Uint8 pixelColor =
				*(
					(Uint8*)pLevelTex->pixels +
					pLevelTex->pitch + (pLevelTex->h - 2) * pLevelTex->pitch - (y * pLevelTex->pitch) +
					x * pLevelTex->format->BytesPerPixel
					);

			if (pixelColor == 255)
			{
				//if the pixel is white spawn a road at that grid cell.
				GameObject* go = new GameObject(fmt::format("Grid-{}_{}.Road", x, y));
				go->GetTransform().SetPosition(pos);

				//Floor texture renderer
				go->AddComponent(new TextureRenderComponent(resources.LoadTexture("Textures/Spritesheet.png"), { 0.f, 0.f }, floorSourceRect));

				pScene->AddGameObject(go);
			}
			else
			{
				//pixel is black, spawn a wall at the grid cell.
				GameObject* go = new GameObject(fmt::format("Grid-{}_{}.Wall", x, y));
				go->GetTransform().SetPosition(pos);
				go->AddTag("wall");
				//invisible box collider.
				go->AddComponent(new BoxColliderComponent({ m_CellSize, m_CellSize }, true, { 0.f, 0.f }));

#ifdef _DEBUG
				go->GetComponent<BoxColliderComponent>()->EnableDebugDrawing();
#endif //_DEBUG

				pScene->AddGameObject(go);

				//Mark this cell as wall in the pathfinding graph
				const int nodeIdx = m_pGridGraph->GetIndex(x, y);
				m_pGridGraph->GetNode(nodeIdx)->SetTerrainType(graph::TerrainType::Wall);
				m_pGridGraph->RemoveConnectionsToAdjacentNodes(nodeIdx);
				m_pGridGraph->AddConnectionsToAdjacentCells(nodeIdx);
			}
		}
	}
	SDL_FreeSurface(pLevelTex);
}

void Tron::CreateLevelBoundsColliders(Scene* pScene)
{
	const Rectf::Vertices playFieldBounds = Rectf{ m_GridStart.x, m_GridStart.y, m_GridDimensions.x * m_CellSize, m_GridDimensions.y * m_CellSize }.GetVertices();
	GameObject* pLevelCollidersGO = new GameObject("LevelColliders");

	//LEFT
	{
		GameObject* go = new GameObject("LEFT");

		go->AddComponent
		(
			new BoxColliderComponent
			(
				{ m_BoundsColliderSize, playFieldBounds.topRight.y - playFieldBounds.bottomRight.y + m_BoundsColliderSize },
				true,
				{ 1.f, 0.f }
			)
		);
		go->AddTag("wall");

#ifdef _DEBUG
			go->GetComponent<BoxColliderComponent>()->EnableDebugDrawing();
#endif //_DEBUG

			go->GetTransform().SetPosition(playFieldBounds.bottomLeft);

		pLevelCollidersGO->AddChild(go);
	}

	//RIGHT
	{
		GameObject* go = new GameObject("RIGHT");

		go->AddComponent
		(
			new BoxColliderComponent
			(
				{ m_BoundsColliderSize, playFieldBounds.topRight.y - playFieldBounds.bottomRight.y + m_BoundsColliderSize },
				true,
				{ 0.f, 0.f }
			)
		);
		go->AddTag("wall");

#ifdef _DEBUG
		go->GetComponent<BoxColliderComponent>()->EnableDebugDrawing();
#endif //_DEBUG

		go->GetTransform().SetPosition(playFieldBounds.bottomRight);

		pLevelCollidersGO->AddChild(go);
	}

	//TOP
	{
		GameObject* go = new GameObject("TOP");

		go->AddComponent
		(
			new BoxColliderComponent
			(
				{ playFieldBounds.topRight.x - playFieldBounds.topLeft.x + m_BoundsColliderSize, m_BoundsColliderSize },
				true,
				{ 0.f, 0.f }
			)
		);

		go->AddTag("wall");

#ifdef _DEBUG
		go->GetComponent<BoxColliderComponent>()->EnableDebugDrawing();
#endif //_DEBUG

		go->GetTransform().SetPosition(playFieldBounds.topLeft);

		pLevelCollidersGO->AddChild(go);
	}

	//BOTTOM
	{
		GameObject* go = new GameObject("BOTTOM");

		go->AddComponent
		(
			new BoxColliderComponent
			(
				{ playFieldBounds.topRight.x - playFieldBounds.topLeft.x + m_BoundsColliderSize, m_BoundsColliderSize },
				true,
				{ 0.f, 1.f }
			)
		);

		go->AddTag("wall");

#ifdef _DEBUG
		go->GetComponent<BoxColliderComponent>()->EnableDebugDrawing();
#endif //_DEBUG

		go->GetTransform().SetPosition(playFieldBounds.bottomLeft);

		pLevelCollidersGO->AddChild(go);
	}

	pScene->AddGameObject(pLevelCollidersGO);

}
