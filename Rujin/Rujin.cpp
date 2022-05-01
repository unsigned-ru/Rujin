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

#include "FMOD_AudioProvider.h"
#include "ServiceLocator.h"
#include "SteamAchievementSystem.h"
#include "Rutils/Logger.h"

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
	Logger::Initialize(); //TODO: make a service perhaps.

	InitializeSDL();

	InitializeSteamworks();

	Renderer::GetInstance().Init(m_Window);
}

void rujin::Rujin::InitializeSDL()
{
	PrintSDLVersion();

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
	{
		LOG_ERROR(std::string("SDL_Init Error: ") + SDL_GetError());
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
		LOG_ERROR(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
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
	auto& audioService = ServiceLocator::RegisterService<AudioService, FMOD_AudioProvider>();
	auto& resourceManager = ResourceManager::GetInstance();
	auto* pScene = SceneManager::GetInstance().CreateScene("Demo");

	//load required textures
	const auto pBackgroundTexture = resourceManager.LoadTexture("background.jpg");
	const auto pLogoTexture = resourceManager.LoadTexture("logo.png");

	//load required fonts
	const auto pLinguaFont = resourceManager.LoadFont("Lingua.otf", 20);

	//load sounds
	audioService.LoadSound("Ahem.wav");

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

	/* setup input commands */
	auto& pInput = input::InputManager::GetInstance();

	//p1 commands
	pInput.RegisterCommand(input::ControllerButton::A, std::make_unique<command::PlaySound>("Ahem.wav"));

	/* Print instructions */
	std::cout
		<< "# ------------------ #" << std::endl
		<< "# ---INSTRUCTIONS--- #" << std::endl
		<< "# ------------------ #" << std::endl
		<< "> Controls <" << std::endl
		<< "	Start sound 1:		A" << std::endl
		<< "	Start sound 2:		B" << std::endl
		<< "	Start sound 3:		Y" << std::endl
		<< "	Start sound 4:		X" << std::endl;
}

void rujin::Rujin::Cleanup()
{
	Renderer::GetInstance().Destroy();

	SteamAPI_Shutdown();

	SDL_DestroyWindow(m_Window);
	m_Window = nullptr;
	SDL_Quit();

	Logger::Release();
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

		/* RunThread scene */
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
