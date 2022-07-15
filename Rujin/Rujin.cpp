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

rujin::Rujin::Rujin(IGame* pGame)
{
	//configure engine for selected game.
	pGame->Configure(m_InitParams);

	m_FixedUpdateTimestep = m_InitParams.fixedUpdateTimestep;
}

void rujin::Rujin::Initialize()
{
	Logger::Initialize(); //TODO: make a service perhaps.

	InitializeSDL();

	Renderer::GetInstance().Init(m_Window, m_InitParams);
}

void rujin::Rujin::InitializeSDL()
{
	LOG_DEBUG("Initializing SDL...");
	PrintSDLVersion();

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
		LOG_ERROR(std::string("SDL_Init Error: ") + SDL_GetError());

	m_Window = SDL_CreateWindow(
		m_InitParams.windowTitle.c_str(),
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		m_InitParams.windowSize.x,
		m_InitParams.windowSize.y,
		SDL_WINDOW_OPENGL
	);

	if (m_Window == nullptr)
		LOG_ERROR_("SDL_CreateWindow Error: {}", SDL_GetError());

	SDL_GetWindowSize(m_Window, &m_WindowSize.x, &m_WindowSize.y);

	LOG_DEBUG_("SDL Initialized.");
}

void rujin::Rujin::PrintSDLVersion()
{
	//gets reassigned, don't need to initialize
	SDL_version compiled; 
	SDL_version linked;

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

void rujin::Rujin::Run()
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
	ResourceManager::GetInstance().Init(m_InitParams.resourcePath);

	LOG_DEBUG("Loading Game...");
	m_pGame->Load();

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
		while (lag >= m_FixedUpdateTimestep)
		{
			sceneManager.FixedUpdate();
			lag -= m_FixedUpdateTimestep;
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

void rujin::Rujin::SetVSync(settings::VSyncMode vsyncMode)
{
	SDL_GL_SetSwapInterval(static_cast<int>(vsyncMode));
}
