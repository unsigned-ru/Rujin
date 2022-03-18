#include "RujinPCH.h"
#include "Rujin.h"

#include "Managers/InputManager.h"
#include "Managers/SceneManager.h"
#include "Managers/ResourceManager.h"

#include "Components/DearImGuiComponent.h"
#include "Components/FpsComponent.h"
#include "Components/TextComponent.h"
#include "Components/TextureRenderComponent.h"
#include "Components/PeterPepperComponent.h"
#include "Components/PlayerHUDComponent.h"
#include "Components/TextRenderComponent.h"

#include "GameLoop/Renderer.h"
#include "GameLoop/Scene.h"

#include "Commands.h"

#include <functional>
#include <thread>

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
	PrintSDLVersion();
	
	if (SDL_Init(SDL_INIT_VIDEO) != 0) 
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
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

	Renderer::GetInstance().Init(m_Window);
}

/**
 * Code constructing the scene world starts here
 */
void rujin::Rujin::LoadGame() const
{
	auto* pScene = SceneManager::GetInstance().CreateScene("Demo");

	//load required textures
	const auto pBackgroundTexture = ResourceManager::GetInstance().LoadTexture("background.jpg");
	const auto pLogoTexture = ResourceManager::GetInstance().LoadTexture("logo.png");
	const auto pLinguaFont = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);

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

		// 
		go->AddComponent(new TextRenderComponent(pTextComponent, pTextureRenderComponent));

		pScene->AddGameObject(go);
	}
	

	{
		//create game object
		auto go = std::make_unique<GameObject>("FpsCounter");
		go->GetTransform()->SetPosition({ 10, 10 });

		//add text component
		auto* pTextComponent = go->AddComponent(new TextComponent(pLinguaFont, { 255, 255, 0, 255 }));

		//add texture render component
		go->AddComponent<TextureRenderComponent>();
		go->AddComponent<TextRenderComponent>();

		//add fps component
		go->AddComponent(new FpsComponent(pTextComponent));

		pScene->AddGameObject(go);
	}

	auto p1GO = std::make_unique<GameObject>("Player1");
	auto* const pP1PPC = p1GO->AddComponent(new PeterPepperComponent());
	pScene->AddGameObject(p1GO);

	auto HUD = std::make_unique<GameObject>("HUD");

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
	HUD->AddChild(p1HUD);

	pScene->AddGameObject(HUD);

	/* setup input commands */
	auto& pInput = input::InputManager::GetInstance();

	pInput.RegisterCommand(input::ControllerButton::A, std::make_unique<command::Die>(pP1PPC));
	pInput.RegisterCommand(input::ControllerButton::B, std::make_unique<command::GainPoints>(pP1PPC));
}

void rujin::Rujin::Cleanup()
{
	Renderer::GetInstance().Destroy();
	SDL_DestroyWindow(m_Window);
	m_Window = nullptr;
	SDL_Quit();
}

void rujin::Rujin::Run()
{
	Initialize();

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
	while (doContinue)
	{
		const auto currentTime = chrono::high_resolution_clock::now();
		s_DeltaTime = chrono::duration<float>(currentTime - lastTime).count();
		lastTime = currentTime;
		lag += s_DeltaTime;
		doContinue = input.ProcessInput(); //TODO: don't do this every frame, but in a polling interval
		sceneManager.Update();
		while (lag >= s_FixedTimestamp)
		{
			sceneManager.FixedUpdate();
			lag -= s_FixedTimestamp;
		}
		renderer.Render();
	}

	Cleanup();
}

float rujin::Rujin::GetDeltaTime()
{
	return s_DeltaTime;
}
