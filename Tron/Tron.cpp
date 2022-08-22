#include "TronPCH.h"
#include "Tron.h"

#include <fstream>
#include <SDL_image.h>

#include "BoxColliderComponent.h"
#include "EnemyListComponent.h"
#include "FMOD_AudioProvider.h"
#include "GameObject.h"
#include "HeuristicFunctions.h"
#include "InputService.h"
#include "MainMenuComponent.h"
#include "Prefabs.h"
#include "ResourceProvider.h"
#include "Rujin.h"
#include "Scene.h"
#include "SceneProvider.h"
#include "ServiceLocator.h"
#include "SpawnLocationListComponent.h"
#include "TankComponent.h"
#include "TextRenderComponent.h"
#include "TextureRenderComponent.h"
#include "Rutils/String.h"

using namespace rujin;

void Tron::Configure(settings::InitParams& params)
{
	params.windowTitle = "Rujin Demo: Tron";
	params.windowSize = { 750 , 790 };
}

void Tron::Load()
{
	ServiceLocator::GetService<InputService>().RegisterPlayer();

	LoadMenuScreen();
	LoadLevel1();
	LoadLevel2();
	LoadLevel3();
}

const glm::ivec2& Tron::GetGridDimensions() const
{
	return m_GridDimensions;
}

void Tron::StartGame(GameMode gamemode)
{
	m_GameMode = gamemode;

	const auto& scenes = ServiceLocator::GetService<SceneService>();
	Scene* pLevel1 = scenes.GetScene("Level1");
	
	auto* pManagerObj = pLevel1->GetRootGameObjectByPredicate([](const GameObject* pObj) {return pObj->GetName() == "Manager"; });
	pManagerObj->GetComponent<EnemyListComponent>()->SetDirty();
	auto* spawnLocations = pManagerObj->GetComponent<SpawnLocationListComponent>();
	ServiceLocator::GetService<SceneService>().SetActiveScene(pLevel1);

	const auto& windowSize = Rujin::Get()->GetWindowContext().windowSize;

	/* Create player 1*/
	{
		GameObject* playerGO = prefabs::CreatePlayerTank("Player1", 0);
		playerGO->AddTag("Faction1");

		//calc position
		playerGO->GetTransform().SetPosition(spawnLocations->GetRandomSpawnLocation());

		pLevel1->AddGameObject(playerGO);

		GameObject* pPlayerHUD = prefabs::CreatePlayerHUD("Player1_HUD", playerGO->GetComponent<TronPlayerComponent>(), 25.f, { 0.f, 1.f });
		pPlayerHUD->GetTransform().SetPosition({ 0.f, Rujin::Get()->GetWindowContext().windowSize.y });
		pLevel1->AddGameObject(pPlayerHUD);
	}

	if (m_GameMode == GameMode::CO_OP || m_GameMode == GameMode::PLAYER_VS_PLAYER)
	{
		/* Create player 2*/
		{
			GameObject* playerGO = prefabs::CreatePlayerTank("Player2",  ServiceLocator::GetService<InputService>().RegisterPlayer());
			if (m_GameMode == GameMode::CO_OP)
				playerGO->AddTag("Faction1");
			else
				playerGO->AddTag("Faction2");

			//calc position
			playerGO->GetTransform().SetPosition(spawnLocations->GetRandomSpawnLocation());

			pLevel1->AddGameObject(playerGO);

			GameObject* pPlayerHUD = prefabs::CreatePlayerHUD("Player2_HUD", playerGO->GetComponent<TronPlayerComponent>(), -25.f, {1.f, 1.f});
			pPlayerHUD->GetTransform().SetPosition({ windowSize.x, Rujin::Get()->GetWindowContext().windowSize.y });
			pLevel1->AddGameObject(pPlayerHUD);
		}
	}

	if (m_GameMode != GameMode::PLAYER_VS_PLAYER) 
		SpawnLevel1Enemies(pLevel1);

}

void Tron::SwitchToNextLevel()
{
	m_CurrentLevel = (m_CurrentLevel + 1 - 1) % 3 + 1;

	auto& scenes = ServiceLocator::GetService<SceneService>();

	//get player gameobject and HUD
	const auto currentScene = scenes.GetActiveScene();
	const auto pObjectsToMove = currentScene->GetAllRootGameObjectsByPredicate([](const GameObject* pObj) { return pObj->HasTag("LevelPersistent"); });

	//TODO: remove all bullets

	Scene* pNewScene;
	switch (m_CurrentLevel)
	{
		default:
		case 1:
		{
			pNewScene = scenes.GetScene("Level1");
			SpawnLevel1Enemies(pNewScene);
			break;
		}
		case 2:
		{
			pNewScene = scenes.GetScene("Level2");
			SpawnLevel2Enemies(pNewScene);
			break;
		}
		case 3:
		{
			pNewScene = scenes.GetScene("Level3");
			SpawnLevel3Enemies(pNewScene);
			break;
		}
	}

	for (GameObject* pObjToMove : pObjectsToMove)
		currentScene->MoveGameObject(pObjToMove, pNewScene);

 	pNewScene->GetRootGameObjectByPredicate([](const GameObject* pObj) {return pObj->GetName() == "Manager"; })->GetComponent<EnemyListComponent>()->SetDirty();
	scenes.SetActiveScene(pNewScene);
}

Tron::GameMode Tron::GetGameMode() const
{
	return m_GameMode;
}

void Tron::LoadMenuScreen()
{
	ResourceService& resources = ServiceLocator::GetService<ResourceService>();
	SceneService& scenes = ServiceLocator::GetService<SceneService>();

	/* Create scene with collision field with the size of the screen. */
	const auto& windowSize = Rujin::Get()->GetWindowContext().windowSize;
	Scene* pScene = scenes.CreateScene("MenuScreen", Rectf{ 0, 0, 0, 0 });

	GameObject* pTronLogo = new GameObject("Tron_Logo");
	pTronLogo->AddComponent(new TextureRenderComponent(resources.GetTexture("Textures/TronLogo.png")));
	pTronLogo->GetTransform().SetPosition({ windowSize.x / 2.f, windowSize.y * 0.75f });
	pScene->AddGameObject(pTronLogo);


	GameObject* pMenu = new GameObject("Menu");
	pMenu->GetTransform().SetLocalPosition({ windowSize.x / 2.f, 0.33f * windowSize.y });

	GameObject* pSinglePlayer = new GameObject("SinglePlayerText");
	pSinglePlayer->GetTransform().SetLocalPosition({ 0.f, 75.f });
	auto* pSinglePlayerText = pSinglePlayer->AddComponent(new TextRenderComponent(resources.GetFont("Fonts/Pixeboy.ttf", 50), { 1,1,1,1 }, { 0.5f, 0.5f }));
	pSinglePlayerText->SetText("Single  Player");
	pMenu->AddChild(pSinglePlayer);

	GameObject* pCoOp = new GameObject("CoOpText");
	auto* pCoOpText = pCoOp->AddComponent(new TextRenderComponent(resources.GetFont("Fonts/Pixeboy.ttf", 50), { 1,1,1,1 }, { 0.5f, 0.5f }));
	pCoOpText->SetText("Co-Op");
	pMenu->AddChild(pCoOp);

	GameObject* pPlayerVsPlayer = new GameObject("PlayerVsPlayerText");
	pPlayerVsPlayer->GetTransform().SetLocalPosition({ 0.f, -75.f });
	auto* pPlayerVsPlayerText = pPlayerVsPlayer->AddComponent(new TextRenderComponent(resources.GetFont("Fonts/Pixeboy.ttf", 50), { 1,1,1,1 }, { 0.5f, 0.5f }));
	pPlayerVsPlayerText->SetText("Player  vs  Player");
	pMenu->AddChild(pPlayerVsPlayer);

	pMenu->AddComponent(new MainMenuComponent(pSinglePlayerText, pCoOpText, pPlayerVsPlayerText));

	pScene->AddGameObject(pMenu);
}

void Tron::LoadLevel1()
{
	ResourceService& resourceService = ServiceLocator::GetService<ResourceService>();
	SceneService& sceneService = ServiceLocator::GetService<SceneService>();

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

	/* Create random teleporter */
	{
		auto* go = prefabs::CreateRandomTeleporter("RandomTeleporter");
		go->GetTransform().SetPosition({ m_GridStart.x + (m_GridDimensions.x * m_CellSize) / 2.f, m_GridStart.y + (m_GridDimensions.y * m_CellSize) / 2.f });
		pScene->AddGameObject(go);
	}
}

void Tron::SpawnLevel1Enemies(Scene* pScene)
{
	auto pManager = pScene->GetRootGameObjectByPredicate([](const GameObject* pObj) { return pObj->GetName() == "Manager"; });
	auto* pSpawnLocations = pManager->GetComponent<SpawnLocationListComponent>();
	/* Create Enemy Tank*/
	{
		GameObject* pEnemyGO = prefabs::CreateEnemyTank();
		pEnemyGO->GetTransform().SetPosition(pSpawnLocations->GetRandomSpawnLocation());
		pScene->AddGameObject(pEnemyGO);
	}

	/* Create Enemy Recognizer*/
	{
		GameObject* pEnemyGO = prefabs::CreateEnemyRecognizer();

		//calc position
		pEnemyGO->GetTransform().SetPosition(pSpawnLocations->GetRandomSpawnLocation());

		pScene->AddGameObject(pEnemyGO);
	}
}

void Tron::LoadLevel2()
{
	ResourceService& resourceService = ServiceLocator::GetService<ResourceService>();
	SceneService& sceneService = ServiceLocator::GetService<SceneService>();

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

	/* Create random teleporter */
	{
		auto* go = prefabs::CreateRandomTeleporter("RandomTeleporter");
		go->GetTransform().SetPosition({ m_GridStart.x + (m_GridDimensions.x * m_CellSize) / 2.f, m_GridStart.y + (m_GridDimensions.y * m_CellSize) / 2.f });
		pScene->AddGameObject(go);
	}
}

void Tron::SpawnLevel2Enemies(Scene* pScene)
{
	auto pManager = pScene->GetRootGameObjectByPredicate([](const GameObject* pObj) { return pObj->GetName() == "Manager"; });
	auto* pSpawnLocations = pManager->GetComponent<SpawnLocationListComponent>();

	/* Create Enemy Tank*/
	{
		GameObject* pEnemyGO = prefabs::CreateEnemyTank();


		//calc position
		pEnemyGO->GetTransform().SetPosition(pSpawnLocations->GetRandomSpawnLocation());

		pScene->AddGameObject(pEnemyGO);
	}

	/* Create Enemy Recognizer*/
	{
		GameObject* pEnemyGO = prefabs::CreateEnemyRecognizer();

		//calc position
		pEnemyGO->GetTransform().SetPosition(pSpawnLocations->GetRandomSpawnLocation());

		pScene->AddGameObject(pEnemyGO);
	}
}

void Tron::LoadLevel3()
{
	ResourceService& resourceService = ServiceLocator::GetService<ResourceService>();
	SceneService& sceneService = ServiceLocator::GetService<SceneService>();

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


	/* Create random teleporter */
	{
		auto* go = prefabs::CreateRandomTeleporter("RandomTeleporter");
		go->GetTransform().SetPosition({ m_GridStart.x + (m_GridDimensions.x * m_CellSize) / 2.f, m_GridStart.y + (m_GridDimensions.y * m_CellSize) / 2.f });
		pScene->AddGameObject(go);
	}
}

void Tron::SpawnLevel3Enemies(Scene* pScene)
{
	auto pManager = pScene->GetRootGameObjectByPredicate([](const GameObject* pObj) { return pObj->GetName() == "Manager"; });
	auto* pSpawnLocations = pManager->GetComponent<SpawnLocationListComponent>();

	/* Create Enemy Tank*/
	{
		GameObject* pEnemyGO = prefabs::CreateEnemyTank();

		//calc position
		pEnemyGO->GetTransform().SetPosition(pSpawnLocations->GetRandomSpawnLocation());

		pScene->AddGameObject(pEnemyGO);
	}

	/* Create Enemy Recognizer*/
	{
		GameObject* pEnemyGO = prefabs::CreateEnemyRecognizer();

		//calc position
		pEnemyGO->GetTransform().SetPosition(pSpawnLocations->GetRandomSpawnLocation());

		pScene->AddGameObject(pEnemyGO);
	}
}

void Tron::LoadHighScoreScene(uint32_t newScore)
{
	ResourceService& resources = ServiceLocator::GetService<ResourceService>();
	SceneService& scenes = ServiceLocator::GetService<SceneService>();

	/*Save new highscore*/
	std::ofstream ofstream{ fmt::format("../Data/highscores_{}.txt", static_cast<uint8_t>(m_GameMode)), std::fstream::out | std::fstream::app };
	ofstream << newScore << ";";
	ofstream.close();

	/*Load highscores from file*/
	std::vector<uint32_t> highscores = LoadHighScoresFromFile(fmt::format("../Data/highscores_{}.txt", static_cast<uint8_t>(m_GameMode)));

	/* Create scene with collision field with the size of the screen. */
	const auto& windowSize = Rujin::Get()->GetWindowContext().windowSize;
	Scene* pScene = scenes.CreateScene("HighscoreScene", Rectf{ 0, 0, 0, 0 });

	const float startHeight{ windowSize.y * 0.8f };
	const float xPos{ windowSize.x / 2.f - 75.f };
	const float offset{ 50.f };

	const auto scoreCount = std::min(10, static_cast<int>(highscores.size()));

	GameObject* pTitle = new GameObject("Title");
	pTitle->AddComponent(new TextRenderComponent(resources.GetFont("Fonts/Pixeboy.ttf", 45), {1,1,1,1}, {0.5f, 0.5f}))->SetText("Highscores");
	pTitle->GetTransform().SetLocalPosition({ windowSize.x / 2.f, windowSize.y * 0.9f});
	pScene->AddGameObject(pTitle);

	GameObject* pTitleUnderscore = new GameObject("Title_Underscore");
	pTitleUnderscore->AddComponent(new TextRenderComponent(resources.GetFont("Fonts/Pixeboy.ttf", 45), { 1,1,1,1 }, { 0.5f, 0.5f }))->SetText("-----------");
	pTitleUnderscore->GetTransform().SetLocalPosition({ windowSize.x / 2.f, windowSize.y * 0.86f });
	pScene->AddGameObject(pTitleUnderscore);

	for (int i = 0; i < scoreCount; ++i)
	{
		GameObject* pGo = new GameObject("highscore_" + std::to_string(i));
		pGo->AddComponent(new TextRenderComponent(resources.GetFont("Fonts/Pixeboy.ttf", 37)))->SetText(fmt::format("{}. {}", i+1, highscores[i]));
		pGo->GetTransform().SetLocalPosition({ xPos, startHeight - i * offset });
		pScene->AddGameObject(pGo);
	}

	GameObject* pYourScore = new GameObject("YourScore");
	pYourScore->AddComponent(new TextRenderComponent(resources.GetFont("Fonts/Pixeboy.ttf", 37), { 1,1,1,1 }, { 0.5f, 0.5f }))->SetText("Your Score: " + std::to_string(newScore));
	pYourScore->GetTransform().SetLocalPosition({ windowSize.x / 2.f, windowSize.y * 0.1f });
	pScene->AddGameObject(pYourScore);

	scenes.SetActiveScene(pScene);
}

void Tron::LoadGameOverScene()
{
	ResourceService& resources = ServiceLocator::GetService<ResourceService>();
	SceneService& scenes = ServiceLocator::GetService<SceneService>();


	/* Create scene with collision field with the size of the screen. */
	const auto& windowSize = Rujin::Get()->GetWindowContext().windowSize;
	Scene* pScene = scenes.CreateScene("GameOverScene", Rectf{ 0, 0, 0, 0 });

	GameObject* pTitle = new GameObject("Game Over");
	pTitle->AddComponent(new TextRenderComponent(resources.GetFont("Fonts/Pixeboy.ttf", 100), { 1,1,1,1 }, { 0.5f, 0.5f }))->SetText("Game Over");
	pTitle->GetTransform().SetLocalPosition({ windowSize.x / 2.f, windowSize.y * 0.6f });
	pScene->AddGameObject(pTitle);


	scenes.SetActiveScene(pScene);
}

std::vector<uint32_t> Tron::LoadHighScoresFromFile(const std::string& filename)
{
	const std::ifstream stream(filename);
	std::stringstream buffer;
	buffer << stream.rdbuf();

	//highscores as strings:
	const std::vector<std::string> stringHighscores = rutils::SplitStringByDelimiters(buffer.str(), ";");

	std::vector<uint32_t> numberHighscores{};
	numberHighscores.reserve(stringHighscores.size());

	for (const std::string& stringScore  : stringHighscores)
		numberHighscores.push_back(atoi(stringScore.c_str()));

	std::sort(numberHighscores.begin(), numberHighscores.end(), std::greater<int>());


	return numberHighscores;
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