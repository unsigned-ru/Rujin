#include "RujinPCH.h"
#include "Rujin.h"

#include "InputManager.h"
#include "ResourceManager.h"
#include "SceneManager.h"


#include "DearImGuiComponent.h"
#include "FpsComponent.h"
#include "TextComponent.h"
#include "TextRenderComponent.h"
#include "TextureRenderComponent.h"
#include "PlayerInputTestComponent.h"

#include "SDL_AudioProvider.h"

#include "Renderer.h"
#include "Scene.h"

#include <fstream>
#include <functional>
#include <thread>

#include "FMOD_AudioProvider.h"
#include "ServiceLocator.h"
#include "Rutils/Logger.h"
#include "InputActions.h"

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
	audioService.LoadAudio("Audio/SFX/ahem.wav");
	audioService.LoadAudio("Audio/SFX/phone_ring.wav");
	audioService.LoadAudio("Audio/SFX/rooster_song.wav");
	audioService.LoadAudio("Audio/SFX/message.wav");

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

	auto& im = input::InputManager::GetInstance();
	const PlayerIndex p1Index = im.RegisterPlayer();

	{
		//create game object
		auto go = std::make_unique<GameObject>("Player1");

		//add fps component
		go->AddComponent(new PlayerInputTestComponent(p1Index));

		pScene->AddGameObject(go);
	}

	im.AddInputAction(p1Index, (uint32_t)InputAction::StartSound1, InputActionKeybinds(ButtonState::Released, 'X', GamepadButton::X));
	im.AddInputAction(p1Index, (uint32_t)InputAction::StopSound1, InputActionKeybinds(ButtonState::Released, 'B', GamepadButton::B));
	im.AddInputAction(p1Index, (uint32_t)InputAction::PauseSound1, InputActionKeybinds(ButtonState::Released, 'Y', GamepadButton::Y));
	im.AddInputAction(p1Index, (uint32_t)InputAction::SwitchSoundSystem, InputActionKeybinds(ButtonState::Released, 'A', GamepadButton::A));


	im.AddAxisAction
	(
		p1Index, 
		(uint32_t)AxisAction::Volume,
		AxisActionKeybinds
		(
			KeyboardAxisKeybinds('Z', 'S'),
			GamepadAxisKeybinds(GamepadButton::RIGHT_TRIGGER, GamepadButton::LEFT_TRIGGER)
		)
	);
	im.AddAxisAction
	(
		p1Index,
		(uint32_t)AxisAction::LookRight,
		AxisActionKeybinds
		(
			KeyboardAxisKeybinds(),
			GamepadAxisKeybinds(GamepadButton::RIGHT_STICK_X),
			MouseAxis::X
		)
	);

	const PlayerIndex p2Index = im.RegisterPlayer();

	{
		//create game object
		auto go = std::make_unique<GameObject>("Player2");

		//add fps component
		go->AddComponent(new PlayerInputTestComponent(p2Index));

		pScene->AddGameObject(go);
	}

	im.AddInputAction(p2Index, (uint32_t)InputAction::StartSound1, InputActionKeybinds(ButtonState::Released, 'x', GamepadButton::X));
	im.AddInputAction(p2Index, (uint32_t)InputAction::StopSound1, InputActionKeybinds(ButtonState::Released, 'b', GamepadButton::B));
	im.AddInputAction(p2Index, (uint32_t)InputAction::PauseSound1, InputActionKeybinds(ButtonState::Released, 'y', GamepadButton::Y));
	im.AddInputAction(p2Index, (uint32_t)InputAction::SwitchSoundSystem, InputActionKeybinds(ButtonState::Released, 'a', GamepadButton::A));
	im.AddAxisAction
	(
		p2Index,
		(uint32_t)AxisAction::Volume,
		AxisActionKeybinds
		(
			KeyboardAxisKeybinds('Z', 'S'),
			GamepadAxisKeybinds(GamepadButton::RIGHT_TRIGGER, GamepadButton::LEFT_TRIGGER)
		)
	);
	im.AddAxisAction
	(
		p2Index,
		(uint32_t)AxisAction::LookRight,
		AxisActionKeybinds
		(
			KeyboardAxisKeybinds(),
			GamepadAxisKeybinds(GamepadButton::RIGHT_STICK_X),
			MouseAxis::X
		)
	);

	/* Print instructions */
	std::cout
		<< "# ------------------ #" << std::endl
		<< "# ---INSTRUCTIONS--- #" << std::endl
		<< "# ------------------ #" << std::endl
		<< "> NOTE: <" << std::endl
		<< "I have implemented both FMOD and SDL_Mixer Audio providers." << std::endl
		<< "Due to limitations of SDL I switched to FMOD as the main audio provider. FMOD already uses it's separate Audio thread and event-queue system, making audio asynchronous." << std::endl
		<< "To prove that i have the skills to make an asynchronous audio system using event-queued requests i implemented a part of the SDL provider." << std::endl
		<< "But not all pure virtuals are implemented in SDL because SDL's does not support all the features i want the audio service to have." << std::endl
		<< "So Event-queue and separate thread audio with my own implementation: SDL_AudioProvider.cpp" << std::endl
		<< "And Main Audio implementation i will use with support for more complex things (like pausing, stopping, etc...) FMOD_AudioProvider.cpp (The one that is fully implemented)" << std::endl
		<< std::endl
		<< "To switch between audio systems you can press the MENU button." << std::endl
		<< std::endl
		<< "> Controls: Starting Sound <" << std::endl
		<< "	Start sound 1:		A" << std::endl
		<< "	Start sound 2:		B" << std::endl
		<< "	Start sound 3:		Y" << std::endl
		<< "	Start sound 4:		X" << std::endl
		<< "> Controls: Stopping Sound <" << std::endl
		<< "	Stop sound 1:		DPAD_DOWN" << std::endl
		<< "	Stop sound 2:		DPAD_RIGHT" << std::endl
		<< "	Stop sound 3:		DPAD_UP" << std::endl
		<< "	Stop sound 4:		DPAD_LEFT" << std::endl
		<< "> Controls: Toggle Pause <" << std::endl
		<< "	Pause/Unpause		START_BUTTON" << std::endl
		<< "	Last playing sound:" << std::endl;

	LOG_INFO("ACTIVE AUDIO PROVIDER: FMOD");
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

		/* input */
		input.ProcessInput(s_DeltaTime); //TODO: don't do this every frame, but in a polling interval

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
