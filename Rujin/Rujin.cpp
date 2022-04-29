#include "RujinPCH.h"
#include "Rujin.h"

#include "InputManager.h"
#include "ResourceManager.h"
#include "SceneManager.h"

#include "BurgerComponent.h"
#include "DearImGuiComponent.h"
#include "EnemyComponent.h"
#include "FpsComponent.h"
#include "PeterPepperComponent.h"
#include "PlayerHUDComponent.h"
#include "TextComponent.h"
#include "TextRenderComponent.h"
#include "TextureRenderComponent.h"

#include "SDL_AudioProvider.h"

#include "Renderer.h"
#include "Scene.h"

#include "Commands.h"

#pragma warning(disable:4996) //disable _CRT_SECURE_NO_WARNINGS error
#include <steam_api.h>
#pragma warning(default:4996) //re-enable

#include <fstream>
#include <functional>
#include <thread>

#include "SteamAchievementSystem.h"

namespace rujin
{
	class DearImGuiComponent;
}

float rujin::Rujin::s_DeltaTime{};

using namespace std;

void PrintSDLVersion()
{
	SDL_version compiled{};
	SDL_version linked{};

	SDL_VERSION(&compiled);
	SDL_GetVersion(&linked);
	printf("We compiled against SDL version %d.%d.%d ...\n",
		compiled.major, compiled.minor, compiled.patch);
	printf("We are linking against SDL version %d.%d.%d.\n",
		linked.major, linked.minor, linked.patch);
}

void rujin::Rujin::Initialize()
{
	InitializeSDL();

	InitializeSteamworks();

	Renderer::GetInstance().Init(m_Window);
}

void rujin::Rujin::InitializeSDL()
{
	PrintSDLVersion();

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	//Initialize SDL_mixer
	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
	{
		throw std::runtime_error(std::string("SDL_Init mixer Error: ") + Mix_GetError());
	}

	m_Window = SDL_CreateWindow(
		"Programming 4 assignment",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		630,
		480,
		SDL_WINDOW_OPENGL
	);

	if (m_Window == nullptr)
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	SDL_GetWindowSize(m_Window, &m_WindowSize.x, &m_WindowSize.y);
}

void rujin::Rujin::InitializeSteamworks()
{
	if (!SteamAPI_Init())
	{
		if (SteamAPI_RestartAppIfNecessary(SteamUtils()->GetAppID()))
		{
			throw std::exception("Steam needs to be running to play, restarting with steam...");
		}
		else
			throw std::exception("Steam needs to be running to play, could not automatically start steam. Do you have it installed?");
	}
	else
		std::cout << "Successfully initialized steam." << std::endl;
}


/**
 * Code constructing the scene world starts here
 */
void rujin::Rujin::LoadGame() const
{
	auto* pScene = SceneManager::GetInstance().CreateScene("Demo");

	auto& steamAch = achievement::SteamAchievementSystem::GetInstance();

	//load required textures
	const auto pBackgroundTexture = ResourceManager::GetInstance().LoadTexture("background.jpg");
	const auto pLogoTexture = ResourceManager::GetInstance().LoadTexture("logo.png");
	const auto pLinguaFont = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);

	{
		//TODO: remove, this is for test purposes
		auto audioServiceProvider = services::SDL_AudioProvider();

		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

	{
		//create game object
		auto go = std::make_unique<GameObject>("Background");

		auto* pTextureRenderComp = go->AddComponent<TextureRenderComponent>();
		pTextureRenderComp->SetTexture(pBackgroundTexture);

		//add game object to scene
		pScene->AddGameObject(go);
	}

	{
		//create game object
		auto go = std::make_unique<GameObject>("Logo");
		go->GetTransform()->SetPosition({ 216, 180 });

		auto* pTextureRenderComp = go->AddComponent<TextureRenderComponent>();
		pTextureRenderComp->SetTexture(pLogoTexture);

		//add game object to scene
		pScene->AddGameObject(go);
	}

	{
		//create game object
		auto go = std::make_unique<GameObject>("Title");
		go->GetTransform()->SetPosition({ 180, 20 });

		//add text component
		auto* pTextComponent = go->AddComponent(new TextComponent(pLinguaFont, { 255, 255, 0, 255 }));
		pTextComponent->SetText("Programming 4 Assignment");

		//add texture render component
		auto* pTextureRenderComponent = go->AddComponent<TextureRenderComponent>();
		pTextureRenderComponent->SetTexture(pTextComponent->GenerateTexture());

		go->AddComponent(new TextRenderComponent(pTextComponent, pTextureRenderComponent));

		pScene->AddGameObject(go);
	}
	

	{
		//create game object
		auto go = std::make_unique<GameObject>("FpsCounter");
		go->GetTransform()->SetPosition({ 10, 10 });

		//add text component
		auto* pTextComponent = go->AddComponent(new TextComponent(pLinguaFont, { 255, 255, 0, 255 }));
		go->AddComponent<TextureRenderComponent>();

		go->AddComponent<TextRenderComponent>();

		//add fps component
		go->AddComponent(new FpsComponent(pTextComponent));

		pScene->AddGameObject(go);
	}

	auto p1GO = std::make_unique<GameObject>("Player1");
	auto* const pP1PPC = p1GO->AddComponent(new PeterPepperComponent({ &steamAch }));
	pScene->AddGameObject(p1GO);

	auto p2GO = std::make_unique<GameObject>("Player2");
	auto* const pP2PPC = p2GO->AddComponent(new PeterPepperComponent({&steamAch }));
	pScene->AddGameObject(p2GO);

	auto pHUD = std::make_unique<GameObject>("HUD");

		auto p1HUD = std::make_unique<GameObject>("Player1HUD");
		p1HUD->GetTransform()->SetPosition({ 20.f, m_WindowSize.y / 2.f });

			auto pP1Lives = std::make_unique<GameObject>("Player1Lives");
			auto* const pP1LivesTextComp = pP1Lives->AddComponent(new TextComponent(pLinguaFont));
			auto* const pP1LivesTextureRenderComp = pP1Lives->AddComponent<TextureRenderComponent>(); //uses default ctor
			pP1Lives->AddComponent(new TextRenderComponent(pP1LivesTextComp, pP1LivesTextureRenderComp));

			auto pP1Points = std::make_unique<GameObject>("Player1Points");
			pP1Points->GetTransform()->SetLocalPosition({ 0, 20.f });
			auto* const pP1PointsTextComp = pP1Points->AddComponent(new TextComponent(pLinguaFont));
			auto* const pP1PointsTextureRenderComp = pP1Points->AddComponent<TextureRenderComponent>(); //uses default ctor
			pP1Points->AddComponent(new TextRenderComponent(pP1PointsTextComp, pP1PointsTextureRenderComp));

		p1HUD->AddComponent(new PlayerHUDComponent(pP1LivesTextComp, pP1PointsTextComp, pP1PPC, { 255, 255, 0, 255 }));
		p1HUD->AddChild(pP1Lives);
		p1HUD->AddChild(pP1Points);

		auto p2HUD = std::make_unique<GameObject>("Player2HUD");
		p2HUD->GetTransform()->SetPosition({ 20.f, m_WindowSize.y / 2.f + 75.f });

			auto pP2Lives = std::make_unique<GameObject>("Player2Lives");
			auto* const pP2LivesTextComp = pP2Lives->AddComponent(new TextComponent(pLinguaFont));
			auto* const pP2LivesTextureRenderComp = pP2Lives->AddComponent<TextureRenderComponent>(); //uses default ctor
			pP2Lives->AddComponent(new TextRenderComponent(pP2LivesTextComp, pP2LivesTextureRenderComp));

			auto pP2Points = std::make_unique<GameObject>("Player2Points");
			pP2Points->GetTransform()->SetLocalPosition({ 0, 20.f });
			auto* const pP2PointsTextComp = pP2Points->AddComponent(new TextComponent(pLinguaFont));
			auto* const pP2PointsTextureRenderComp = pP2Points->AddComponent<TextureRenderComponent>(); //uses default ctor
			pP2Points->AddComponent(new TextRenderComponent(pP2PointsTextComp, pP2PointsTextureRenderComp));

		p2HUD->AddComponent(new PlayerHUDComponent(pP2LivesTextComp, pP2PointsTextComp, pP2PPC, { 255, 255, 0, 255 }));
		p2HUD->AddChild(pP2Lives);
		p2HUD->AddChild(pP2Points);

	pHUD->AddChild(p1HUD);
	pHUD->AddChild(p2HUD);

	pScene->AddGameObject(pHUD);

	auto pBurgerEnemtGO = std::make_unique<GameObject>("BurgerEnemyTestGameObject");
	auto* pEnemyComp = pBurgerEnemtGO->AddComponent<EnemyComponent>();
	auto* pBurgerComp = pBurgerEnemtGO->AddComponent<BurgerComponent>();

	/* setup input commands */
	auto& pInput = input::InputManager::GetInstance();

	//p1 commands
	pInput.RegisterCommand(input::ControllerButton::A, std::make_unique<command::Die>(pP1PPC));
	pInput.RegisterCommand(input::ControllerButton::B, std::make_unique<command::DropBurger>(pP1PPC, pBurgerComp));
	pInput.RegisterCommand(input::ControllerButton::X, std::make_unique<command::KillEnemy>(pP1PPC, pEnemyComp));

	//p2 commands
	pInput.RegisterCommand(input::ControllerButton::DPAD_DOWN, std::make_unique<command::Die>(pP2PPC));
	pInput.RegisterCommand(input::ControllerButton::DPAD_RIGHT, std::make_unique<command::DropBurger>(pP2PPC, pBurgerComp));
	pInput.RegisterCommand(input::ControllerButton::DPAD_LEFT, std::make_unique<command::KillEnemy>(pP2PPC, pEnemyComp));

	/* Print instructions */
	std::cout
		<< "# ------------------ #" << std::endl
		<< "# ---INSTRUCTIONS--- #" << std::endl
		<< "# ------------------ #" << std::endl
		<< "Note: For now all input is done on 1 controller, adding support for multiple controllers and keyboard input asap." << std::endl
		<< "> Player 1 Controls <" << std::endl
		<< "	Die:			A" << std::endl
		<< "	DropBurger:		B" << std::endl
		<< "	KillEnemy:		X" << std::endl
		<< "> Player 2 Controls <" << std::endl
		<< "	Die:			DPAD_DOWN" << std::endl
		<< "	DropBurger:		DPAD_RIGHT" << std::endl
		<< "	KillEnemy:		DPAD_LEFT" << std::endl;
}

void rujin::Rujin::Cleanup()
{
	Renderer::GetInstance().Destroy();

	SteamAPI_Shutdown();

	SDL_DestroyWindow(m_Window);
	m_Window = nullptr;
	SDL_Quit();
}

void rujin::Rujin::Run()
{
	//if initialization fails, catch it, log, cleanup and exit.
	try
	{
		Initialize();
	}
	catch (const std::exception& e)
	{
		std::cerr << "Initialization failed: " << e.what() << std::endl;
		Cleanup();
		return;
	}

	// tell the resource manager where he can find the game data
	ResourceManager::GetInstance().Init("../Data/");

	LoadGame();

	const auto& renderer = Renderer::GetInstance();
	auto& sceneManager = SceneManager::GetInstance();
	const auto& input = input::InputManager::GetInstance();

	sceneManager.Start();
	sceneManager.LateStart();

	bool doContinue = true;
	auto lastTime = chrono::high_resolution_clock::now();
	float lag = 0.0f;

	SDL_Event event;
	while (doContinue)
	{
		/* Poll for SDL_Events (Not doing this makes the SLD window block) */
		while (SDL_PollEvent(&event) > 0)
		{
			switch (event.type)
			{
			case SDL_QUIT:
				doContinue = false;
				break;

			default: break;
			}
		}

		/* update timing */
		const auto currentTime = chrono::high_resolution_clock::now();
		s_DeltaTime = chrono::duration<float>(currentTime - lastTime).count();
		lastTime = currentTime;
		lag += s_DeltaTime;

		/* Dispatch all STEAM_API callbacks */
		SteamAPI_RunCallbacks();

		/* input */
		input.ProcessInput(); //TODO: don't do this every frame, but in a polling interval

		/* Update scene */
		sceneManager.Update();

		/* Fixed updates */
		while (lag >= s_FixedTimestamp)
		{
			sceneManager.FixedUpdate();
			lag -= s_FixedTimestamp;
		}

		/* render */
		renderer.Render();
	}

	Cleanup();
}

float rujin::Rujin::GetDeltaTime()
{
	return s_DeltaTime;
}
