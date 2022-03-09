#include "RujinPCH.h"
#include "Rujin.h"

#include <functional>
#include <thread>

#include "FpsComponent.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "GameObjectFactory.h"
#include "Scene.h"

#include "TextComponent.h"
#include "TextureRenderComponent.h"


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
		1280,
		720,
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
	const auto pLinguaFont = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

	{
		//create game object
		auto* go = new GameObject("Background");

		auto* pTextureRenderComp = go->AddComponent<TextureRenderComponent>();
		pTextureRenderComp->SetTexture(pBackgroundTexture);

		//add game object to scene
		pScene->AddGameObject(go);
	}

	{
		//create game object
		auto* go = new GameObject("Logo");
		go->GetTransform()->SetPosition({ 216, 180 });

		auto* pTextureRenderComp = go->AddComponent<TextureRenderComponent>();
		pTextureRenderComp->SetTexture(pLogoTexture);

		//add game object to scene
		pScene->AddGameObject(go);
	}

	{
		//create game object
		auto* go = new GameObject("Title");
		go->GetTransform()->SetPosition({ 80, 20 });

		//add text component
		auto* pTextComponent = go->AddComponent<TextComponent>();
		pTextComponent->SetText("Programming 4 Assignment");
		pTextComponent->SetFont(pLinguaFont);
		pTextComponent->SetColor({ 255, 255, 255, 255 });

		//add texture render component
		auto* pTextureRenderComponent = go->AddComponent<TextureRenderComponent>();
		pTextureRenderComponent->SetTexture(pTextComponent->GenerateTexture());

		pScene->AddGameObject(go);
	}
	

	{
		//create game object
		auto* go = new GameObject("FpsCounter");
		go->GetTransform()->SetPosition({ 10, 10 });

		//add text component
		auto* pTextComponent = go->AddComponent<TextComponent>();
		pTextComponent->SetFont(pLinguaFont);
		pTextComponent->SetColor({ 255, 255, 0, 255 });

		//add texture render component
		auto* pTextureRenderComponent = go->AddComponent<TextureRenderComponent>();

		//add fps component
		go->AddComponent(new FpsComponent(pTextComponent, pTextureRenderComponent));

		pScene->AddGameObject(go);
	}
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
	auto& input = InputManager::GetInstance();

	sceneManager.Start();

	bool doContinue = true;
	auto lastTime = chrono::high_resolution_clock::now();
	float lag = 0.0f;
	while (doContinue)
	{
		const auto currentTime = chrono::high_resolution_clock::now();
		s_DeltaTime = chrono::duration<float>(currentTime - lastTime).count();
		lastTime = currentTime;
		lag += s_DeltaTime;
		doContinue = input.ProcessInput();
		sceneManager.Update();
		while (lag >= s_FixedTimestamp)
		{
			sceneManager.FixedUpdate();
			lag -= s_FixedTimestamp;
		}
		sceneManager.OnGui(m_Window);
		renderer.Render();
	}

	Cleanup();
}

float rujin::Rujin::GetDeltaTime()
{
	return s_DeltaTime;
}
