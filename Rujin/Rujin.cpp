#include "RujinPCH.h"
#include "Rujin.h"

#include <functional>
#include <thread>

#include "FontComponent.h"
#include "FpsComponent.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "GameObjectFactory.h"
#include "Scene.h"

#include "TextureComponent.h"
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
	auto scene = SceneManager::GetInstance().CreateScene("Demo");

	//create game object
	auto go = GameObjectFactory::CreateEmpty("Background");

	//add texture component
	auto textureComponent = go->AddComponent<TextureComponent>().lock();
	auto texture = ResourceManager::GetInstance().LoadTexture("background.jpg");
	textureComponent->SetTexture(texture);

	//add texture render component
	auto textureRenderComponent = go->AddComponent<TextureRenderComponent>().lock();
	textureRenderComponent->SetTexture(texture);

	//add game object to scene
	scene.lock()->AddGameObject(go);

	//create game object
	go = GameObjectFactory::CreateEmpty("Logo");
	go->GetTransform().lock()->SetPosition({ 216, 180 });

	//add texture component
	textureComponent = go->AddComponent<TextureComponent>().lock();;
	texture = ResourceManager::GetInstance().LoadTexture("logo.png");
	textureComponent->SetTexture(texture);

	//add texture render component
	textureRenderComponent = go->AddComponent<TextureRenderComponent>().lock();
	textureRenderComponent->SetTexture(texture);


	//add game object to scene
	scene.lock()->AddGameObject(go);

	//create game object
	go = GameObjectFactory::CreateCanvas("Text");
	go->GetTransform().lock()->SetPosition({ 80, 20 });

	//add font component
	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto fontComponent = go->AddComponent<FontComponent>().lock();
	fontComponent->SetFont(font);

	//add text component
	auto textComponent = go->AddComponent<TextComponent>().lock();
	textComponent->SetText("Programming 4 Assignment");
	textComponent->SetFont(fontComponent->GetFont());
	textComponent->SetColor({ 255, 255, 255, 255 });

	//add texture render component
	textureRenderComponent = go->AddComponent<TextureRenderComponent>().lock();
	textureRenderComponent->SetTexture(textComponent->GetTexture());

	scene.lock()->AddGameObject(go);

	//create game object
	go = GameObjectFactory::CreateCanvas("FpsCounter");
	go->GetTransform().lock()->SetPosition({ 10, 10 });

	//add font component
	font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 18);
	fontComponent = go->AddComponent<FontComponent>().lock();
	fontComponent->SetFont(font);

	//add text component
	textComponent = go->AddComponent<TextComponent>().lock();
	textComponent->SetText("00 FPS");
	textComponent->SetFont(fontComponent->GetFont());
	textComponent->SetColor({ 255, 255, 0, 255 });

	//add texture render component
	textureRenderComponent = go->AddComponent<TextureRenderComponent>().lock();
	textureRenderComponent->SetTexture(textComponent->GetTexture());

	//add fps component
	const auto fpsComponent = go->AddComponent<FpsComponent>().lock();
	fpsComponent->SetTextComponent(textComponent);
	fpsComponent->SetTextureRenderComponent(textureRenderComponent);

	scene.lock()->AddGameObject(go);
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

	auto& renderer = Renderer::GetInstance();
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
		renderer.Render();
	}

	Cleanup();
}

float rujin::Rujin::GetDeltaTime()
{
	return s_DeltaTime;
}
