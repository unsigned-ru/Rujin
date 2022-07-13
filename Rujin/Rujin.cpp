#include "RujinPCH.h"
#include "Rujin.h"

#include <chrono>

#include "IGame.h"
#include "InputManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "SceneManager.h"

float rujin::Rujin::s_DeltaTime{};

using namespace std;

void rujin::Rujin::Initialize()
{
	Logger::Initialize(); //TODO: make a service perhaps.

	InitializeSDL();

	Renderer::GetInstance().Init(m_Window);
}

void rujin::Rujin::InitializeSDL()
{
	LOG_DEBUG("Initializing SDL...");
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

	LOG_DEBUG_("SDL Initialized.");
}

void rujin::Rujin::PrintSDLVersion()
{
	SDL_version compiled{};
	SDL_version linked{};

	SDL_VERSION(&compiled);
	SDL_GetVersion(&linked);
	LOG_DEBUG_("Compiled against SDL version {}.{}.{} ...\n", compiled.major, compiled.minor, compiled.patch);
	LOG_DEBUG_("Linking against SDL version {}.{}.{}.\n", linked.major, linked.minor, linked.patch);
}

void rujin::Rujin::Cleanup()
{
	Renderer::GetInstance().Destroy();

	SDL_DestroyWindow(m_Window);
	m_Window = nullptr;
	SDL_Quit();

	Logger::Release();
}

void rujin::Rujin::Run(IGame* pGame, const InitializationParameters& params)
{
	//if initialization fails, catch it, log, cleanup and exit.
	try
	{
		Initialize();
	}
	catch (const std::exception& e)
	{
		LOG_ERROR_("Initialization failed: {}", e.what());
		Cleanup();
		return;
	}

	// tell the resource manager where he can find the game data
	ResourceManager::GetInstance().Init(params.resourcePath);

	LOG_DEBUG("Loading Game...");
	pGame->Load();

	const auto& renderer = Renderer::GetInstance();
	auto& sceneManager = SceneManager::GetInstance();
	const auto& input = input::InputManager::GetInstance();
	LOG_DEBUG("Loading Complete.");

	LOG_DEBUG("Starting Game...");
	sceneManager.Start();
	sceneManager.LateStart();
	LOG_DEBUG("Game Started.");

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

		/* input */
		input.ProcessInput(s_DeltaTime);

		/* Update scene */
		sceneManager.Update();

		/* Fixed update(s) */
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
