#include "RujinPCH.h"
#include "Rujin.h"

#include <chrono>

#include "MainRenderProvider.h"
#include "ResourceProvider.h"
#include "SceneProvider.h"
#include "ServiceLocator.h"
#include "FMOD_AudioProvider.h"
#include "XInputProvider.h"

#include "IGame.h"

rujin::Rujin::Rujin(IGame* pGame)
	: m_pGame(pGame)
{
	//configure engine for selected game.
	pGame->Configure(m_InitParams);

	m_FixedUpdateTimestep = m_InitParams.fixedUpdateTimestep;
}

rujin::Rujin::~Rujin()
{
	ServiceLocator::ClearServices();

	SDL_DestroyWindow(m_WindowContext.pWindow);
	m_WindowContext.pWindow = nullptr;
	SDL_Quit();

	Logger::Release();
}

void rujin::Rujin::Initialize()
{
	Logger::Initialize();
	InitializeSDL();

	//initialize default services
	ServiceLocator::RegisterService<RenderService, MainRenderProvider>();
	ServiceLocator::RegisterService<InputService, XInputProvider>();
	ServiceLocator::RegisterService<ResourceService, ResourceProvider>();
	ServiceLocator::RegisterService<AudioService, FMOD_AudioProvider>();
	ServiceLocator::RegisterService<SceneService, SceneProvider>();
}

void rujin::Rujin::InitializeSDL()
{
	LOG_DEBUG("Initializing SDL...");
	PrintSDLVersion();

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
		LOG_ERROR(std::string("SDL_Init Error: ") + SDL_GetError());

	// Use OpenGL 2.1
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

	m_WindowContext.pWindow = SDL_CreateWindow(
		m_InitParams.windowTitle.c_str(),
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		m_InitParams.windowSize.x,
		m_InitParams.windowSize.y,
		SDL_WINDOW_OPENGL
	);

	if (m_WindowContext.pWindow == nullptr)
		LOG_ERROR_("SDL_CreateWindow Error: {}", SDL_GetError());

	SDL_GetWindowSize(m_WindowContext.pWindow, &m_WindowContext.windowSize.x, &m_WindowContext.windowSize.y);
	LOG_DEBUG("SDL Initialized.");
}

void rujin::Rujin::PrintSDLVersion()
{
	//gets reassigned, don't need to initialize
	SDL_version compiled; 
	SDL_version linked;

	SDL_VERSION(&compiled);
	SDL_GetVersion(&linked);
	LOG_DEBUG_("Compiled against SDL version {}.{}.{} ...", compiled.major, compiled.minor, compiled.patch);
	LOG_DEBUG_("Linking against SDL version {}.{}.{}.", linked.major, linked.minor, linked.patch);
}

void rujin::Rujin::Run()
{
	LOG_DEBUG("Loading Game...");
	m_pGame->Load();

	const RenderService& renderer = ServiceLocator::GetService<RenderService>();
	SceneService& sceneService = ServiceLocator::GetService<SceneService>();
	InputService& input = ServiceLocator::GetService<InputService>();

	LOG_DEBUG("Loading Complete.");

	LOG_DEBUG("Starting Game...");
	sceneService.ProcessAdditionsAndRemovals();
	sceneService.Start();
	input.Start();
	sceneService.LateStart();
	LOG_DEBUG("Game Started.");

	bool doContinue = true;
	auto lastTime = std::chrono::high_resolution_clock::now();
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
		const auto currentTime = std::chrono::high_resolution_clock::now();
		m_DeltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
		m_TotalTime += m_DeltaTime;
		lastTime = currentTime;
		lag += m_DeltaTime;

		/* input */
		input.ProcessInput();

		/* Update scene */
		sceneService.Update();

		/* Fixed update(s) */
		while (lag >= m_FixedUpdateTimestep)
		{
			sceneService.FixedUpdate();
			lag -= m_FixedUpdateTimestep;
		}

		sceneService.ProcessAdditionsAndRemovals();

		/* render */
		renderer.Render();
	}

	Destroy();
}

void rujin::Rujin::SetVSync(VSyncMode vsyncMode)
{
	if (SDL_GL_SetSwapInterval(static_cast<int>(vsyncMode)) == 0)
		m_WindowContext.vSyncMode = vsyncMode;
}
