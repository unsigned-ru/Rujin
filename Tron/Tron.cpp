#include "TronPCH.h"
#include "Tron.h"

#include <SDL_image.h>

#include "BoxColliderComponent.h"
#include "EnemyListComponent.h"
#include "FMOD_AudioProvider.h"
#include "GameObject.h"
#include "HeuristicFunctions.h"
#include "InputService.h"
#include "Prefabs.h"
#include "ResourceProvider.h"
#include "Rujin.h"
#include "Scene.h"
#include "SceneProvider.h"
#include "ServiceLocator.h"
#include "SpawnLocationListComponent.h"
#include "TankComponent.h"
#include "TextureRenderComponent.h"

using namespace rujin;

void Tron::Configure(settings::InitParams& params)
{
	params.windowTitle = "Rujin Demo: Tron";
	params.windowSize = { 750 , 790 };
}

void Tron::Load()
{
	//SceneService& sceneService = ServiceLocator::GetService<SceneService>();

	LoadLevel1();
	LoadLevel2();
	LoadLevel3();


}

const glm::ivec2& Tron::GetGridDimensions() const
{
	return m_GridDimensions;
}

void Tron::LoadLevel1()
{
	ResourceService& resourceService = ServiceLocator::GetService<ResourceService>();
	SceneService& sceneService = ServiceLocator::GetService<SceneService>();
	InputService& input = ServiceLocator::GetService<InputService>();

	/* Create scene with collision field with the size of the screen. */
	const auto& windowSize = Rujin::Get()->GetWindowContext().windowSize;
	Scene* pScene = sceneService.CreateScene("Level1", Rectf{ 0, 0, static_cast<float>(windowSize.x), static_cast<float>(windowSize.y) });

	GameObject* pManagerObj = new GameObject("Manager");
	pManagerObj->AddComponent(new EnemyListComponent());
	auto* pSpawnList = pManagerObj->AddComponent(new SpawnLocationListComponent());
	pScene->AddGameObject(pManagerObj);

	///* Create Circuit background */
	{
		GameObject* go = new GameObject("Circuit_BG");
		go->AddComponent(new TextureRenderComponent(resourceService.GetTexture("Textures/Circuit.png"), { 0.f, 0.f }));
		pScene->AddGameObject(go);
	}

	//create pathfinding 
	auto pGridGraph = new graph::GridGraph<graph::GridTerrainNode, graph::GraphConnection>(m_GridStart, m_GridDimensions.x, m_GridDimensions.y, m_CellSize, false, false);

	/* Generate Level Grid */
	GenerateLevelGridFromTexture(pScene, "../Data/Levels/Level1.png", pGridGraph);

	//Set pathfinding based on grid
	auto pAStar = new AStar(pGridGraph, heuristic_functions::Chebyshev);
	pScene->SetPathfinder(pAStar);

	//generate spawnlocations after generating level grid.
	pSpawnList->GenerateSpawnLocations(pGridGraph);

	/* Create surrounding colliders (so player can't move outside of bounds...*/
	CreateLevelBoundsColliders(pScene);

	/* Create player 1*/
	{
		GameObject* playerGO = prefabs::CreatePlayerTank("Player1", input.RegisterPlayer());

		//calc position
		playerGO->GetTransform().SetPosition
		(
			glm::vec2
			{
				(m_GridStart.x + m_CellSize),
				(m_GridStart.y + m_CellSize)
			}
		);

		pScene->AddGameObject(playerGO);

		GameObject* pPlayerHUD = prefabs::CreatePlayerHUD("Player1_HUD", playerGO->GetComponent<TronPlayerComponent>());
		pPlayerHUD->GetTransform().SetPosition({ 0.f, Rujin::Get()->GetWindowContext().windowSize.y });
		pScene->AddGameObject(pPlayerHUD);
	}


	/* Create random teleporter */
	{
		auto* go = prefabs::CreateRandomTeleporter("RandomTeleporter");
		go->GetTransform().SetPosition({ m_GridStart.x + (m_GridDimensions.x * m_CellSize) / 2.f, m_GridStart.y + (m_GridDimensions.y * m_CellSize) / 2.f });
		pScene->AddGameObject(go);
	}

	pScene->AddGameObject(pManagerObj);

	SpawnLevel1Enemies(pScene);
}

void Tron::SpawnLevel1Enemies(Scene* pScene)
{
	/* Create Enemy Tank*/
	{
		GameObject* pEnemyGO = prefabs::CreateEnemyTank();


		//calc position
		pEnemyGO->GetTransform().SetPosition
		(
			glm::vec2
			{
				(m_GridStart.x + m_GridDimensions.x * m_CellSize) / 2.f,
				(m_GridStart.y + m_GridDimensions.y * m_CellSize) / 2.f
			}
		);

		pScene->AddGameObject(pEnemyGO);
	}

	/* Create Enemy Recognizer*/
	{
		GameObject* pEnemyGO = prefabs::CreateEnemyRecognizer();

		//calc position
		pEnemyGO->GetTransform().SetPosition
		(
			glm::vec2
			{
				(m_GridStart.x + m_GridDimensions.x * m_CellSize) - m_CellSize,
				(m_GridStart.y + m_GridDimensions.y * m_CellSize) - m_CellSize
			}
		);

		pScene->AddGameObject(pEnemyGO);
	}
}

void Tron::LoadLevel2()
{
	ResourceService& resourceService = ServiceLocator::GetService<ResourceService>();
	SceneService& sceneService = ServiceLocator::GetService<SceneService>();
	InputService& input = ServiceLocator::GetService<InputService>();

	/* Create scene with collision field with the size of the screen. */
	const auto& windowSize = Rujin::Get()->GetWindowContext().windowSize;
	Scene* pScene = sceneService.CreateScene("Level2", Rectf{ 0, 0, static_cast<float>(windowSize.x), static_cast<float>(windowSize.y) });

	GameObject* pManagerObj = new GameObject("Manager");
	pManagerObj->AddComponent(new EnemyListComponent());
	auto* pSpawnList = pManagerObj->AddComponent(new SpawnLocationListComponent());
	pScene->AddGameObject(pManagerObj);

	///* Create Circuit background */
	{
		GameObject* go = new GameObject("Circuit_BG");
		go->AddComponent(new TextureRenderComponent(resourceService.GetTexture("Textures/Circuit.png"), { 0.f, 0.f }));
		pScene->AddGameObject(go);
	}

	//create pathfinding 
	auto pGridGraph = new graph::GridGraph<graph::GridTerrainNode, graph::GraphConnection>(m_GridStart, m_GridDimensions.x, m_GridDimensions.y, m_CellSize, false, false);

	/* Generate Level Grid */
	GenerateLevelGridFromTexture(pScene, "../Data/Levels/Level2.png", pGridGraph);

	//Set pathfinding based on grid
	auto pAStar = new AStar(pGridGraph, heuristic_functions::Chebyshev);
	pScene->SetPathfinder(pAStar);

	//generate spawnlocations after generating level grid.
	pSpawnList->GenerateSpawnLocations(pGridGraph);

	/* Create surrounding colliders (so player can't move outside of bounds...*/
	CreateLevelBoundsColliders(pScene);

	/* Create player 1*/
	{
		GameObject* playerGO = prefabs::CreatePlayerTank("Player1", input.RegisterPlayer());

		//calc position
		playerGO->GetTransform().SetPosition
		(
			glm::vec2
			{
				(m_GridStart.x + m_CellSize),
				(m_GridStart.y + m_CellSize)
			}
		);

		pScene->AddGameObject(playerGO);

		GameObject* pPlayerHUD = prefabs::CreatePlayerHUD("Player1_HUD", playerGO->GetComponent<TronPlayerComponent>());
		pPlayerHUD->GetTransform().SetPosition({ 0.f, Rujin::Get()->GetWindowContext().windowSize.y });
		pScene->AddGameObject(pPlayerHUD);
	}


	/* Create random teleporter */
	{
		auto* go = prefabs::CreateRandomTeleporter("RandomTeleporter");
		go->GetTransform().SetPosition({ m_GridStart.x + (m_GridDimensions.x * m_CellSize) / 2.f, m_GridStart.y + (m_GridDimensions.y * m_CellSize) / 2.f });
		pScene->AddGameObject(go);
	}

	pScene->AddGameObject(pManagerObj);
}

void Tron::SpawnLevel2Enemies(Scene* pScene)
{
	/* Create Enemy Tank*/
	{
		GameObject* pEnemyGO = prefabs::CreateEnemyTank();


		//calc position
		pEnemyGO->GetTransform().SetPosition
		(
			glm::vec2
			{
				(m_GridStart.x + m_GridDimensions.x * m_CellSize) / 2.f,
				(m_GridStart.y + m_GridDimensions.y * m_CellSize) / 2.f
			}
		);

		pScene->AddGameObject(pEnemyGO);
	}

	/* Create Enemy Recognizer*/
	{
		GameObject* pEnemyGO = prefabs::CreateEnemyRecognizer();

		//calc position
		pEnemyGO->GetTransform().SetPosition
		(
			glm::vec2
			{
				(m_GridStart.x + m_GridDimensions.x * m_CellSize) - m_CellSize,
				(m_GridStart.y + m_GridDimensions.y * m_CellSize) - m_CellSize
			}
		);

		pScene->AddGameObject(pEnemyGO);
	}
}

void Tron::LoadLevel3()
{
	ResourceService& resourceService = ServiceLocator::GetService<ResourceService>();
	SceneService& sceneService = ServiceLocator::GetService<SceneService>();
	InputService& input = ServiceLocator::GetService<InputService>();

	/* Create scene with collision field with the size of the screen. */
	const auto& windowSize = Rujin::Get()->GetWindowContext().windowSize;
	Scene* pScene = sceneService.CreateScene("Level3", Rectf{ 0, 0, static_cast<float>(windowSize.x), static_cast<float>(windowSize.y) });

	GameObject* pManagerObj = new GameObject("Manager");
	pManagerObj->AddComponent(new EnemyListComponent());
	auto* pSpawnList = pManagerObj->AddComponent(new SpawnLocationListComponent());
	pScene->AddGameObject(pManagerObj);

	///* Create Circuit background */
	{
		GameObject* go = new GameObject("Circuit_BG");
		go->AddComponent(new TextureRenderComponent(resourceService.GetTexture("Textures/Circuit.png"), { 0.f, 0.f }));
		pScene->AddGameObject(go);
	}

	//create pathfinding 
	auto pGridGraph = new graph::GridGraph<graph::GridTerrainNode, graph::GraphConnection>(m_GridStart, m_GridDimensions.x, m_GridDimensions.y, m_CellSize, false, false);

	/* Generate Level Grid */
	GenerateLevelGridFromTexture(pScene, "../Data/Levels/Level3.png", pGridGraph);

	//Set pathfinding based on grid
	auto pAStar = new AStar(pGridGraph, heuristic_functions::Chebyshev);
	pScene->SetPathfinder(pAStar);

	//generate spawnlocations after generating level grid.
	pSpawnList->GenerateSpawnLocations(pGridGraph);

	/* Create surrounding colliders (so player can't move outside of bounds...*/
	CreateLevelBoundsColliders(pScene);

	/* Create player 1*/
	{
		GameObject* playerGO = prefabs::CreatePlayerTank("Player1", input.RegisterPlayer());

		//calc position
		playerGO->GetTransform().SetPosition
		(
			glm::vec2
			{
				(m_GridStart.x + m_CellSize),
				(m_GridStart.y + m_CellSize)
			}
		);

		pScene->AddGameObject(playerGO);

		GameObject* pPlayerHUD = prefabs::CreatePlayerHUD("Player1_HUD", playerGO->GetComponent<TronPlayerComponent>());
		pPlayerHUD->GetTransform().SetPosition({ 0.f, Rujin::Get()->GetWindowContext().windowSize.y });
		pScene->AddGameObject(pPlayerHUD);
	}


	/* Create random teleporter */
	{
		auto* go = prefabs::CreateRandomTeleporter("RandomTeleporter");
		go->GetTransform().SetPosition({ m_GridStart.x + (m_GridDimensions.x * m_CellSize) / 2.f, m_GridStart.y + (m_GridDimensions.y * m_CellSize) / 2.f });
		pScene->AddGameObject(go);
	}

	pScene->AddGameObject(pManagerObj);
}

void Tron::SpawnLevel3Enemies(Scene* pScene)
{
	/* Create Enemy Tank*/
	{
		GameObject* pEnemyGO = prefabs::CreateEnemyTank();


		//calc position
		pEnemyGO->GetTransform().SetPosition
		(
			glm::vec2
			{
				(m_GridStart.x + m_GridDimensions.x * m_CellSize) / 2.f,
				(m_GridStart.y + m_GridDimensions.y * m_CellSize) / 2.f
			}
		);

		pScene->AddGameObject(pEnemyGO);
	}

	/* Create Enemy Recognizer*/
	{
		GameObject* pEnemyGO = prefabs::CreateEnemyRecognizer();

		//calc position
		pEnemyGO->GetTransform().SetPosition
		(
			glm::vec2
			{
				(m_GridStart.x + m_GridDimensions.x * m_CellSize) - m_CellSize,
				(m_GridStart.y + m_GridDimensions.y * m_CellSize) - m_CellSize
			}
		);

		pScene->AddGameObject(pEnemyGO);
	}
}

void Tron::GenerateLevelGridFromTexture(Scene* pScene, const std::string& levelTexturePath, graph::GridGraph<graph::GridTerrainNode, graph::GraphConnection>* pGraph)
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
				go->AddComponent(new TextureRenderComponent(resources.GetTexture("Textures/Spritesheet.png"), { 0.f, 0.f }, floorSourceRect));

				pScene->AddGameObject(go);
			}
			else
			{
				//pixel is black, spawn a Wall at the grid cell.
				GameObject* go = new GameObject(fmt::format("Grid-{}_{}.Wall", x, y));
				go->GetTransform().SetPosition(pos);
				go->AddTag("Wall");
				//invisible box collider.
				go->AddComponent(new BoxColliderComponent({ m_CellSize, m_CellSize }, true, { 0.f, 0.f }));

#ifdef _DEBUG
				go->GetComponent<BoxColliderComponent>()->EnableDebugDrawing();
#endif //_DEBUG

				pScene->AddGameObject(go);

				//Mark this cell as Wall in the pathfinding graph
				const int nodeIdx = pGraph->GetIndex(x, y);
				pGraph->GetNode(nodeIdx)->SetTerrainType(graph::TerrainType::Wall);
				pGraph->RemoveConnectionsToAdjacentNodes(nodeIdx);
				pGraph->AddConnectionsToAdjacentCells(nodeIdx);
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
		go->AddTag("Wall");

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
		go->AddTag("Wall");

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

		go->AddTag("Wall");

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

		go->AddTag("Wall");

#ifdef _DEBUG
		go->GetComponent<BoxColliderComponent>()->EnableDebugDrawing();
#endif //_DEBUG

		go->GetTransform().SetPosition(playFieldBounds.bottomLeft);

		pLevelCollidersGO->AddChild(go);
	}

	pScene->AddGameObject(pLevelCollidersGO);

}