#include "TronPCH.h"
#include "Tron.h"

#include "FMOD_AudioProvider.h"
#include "FpsComponent.h"
#include "GameObject.h"
#include "InputActions.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "ServiceLocator.h"
#include "TextComponent.h"
#include "TextRenderComponent.h"
#include "TextureRenderComponent.h"
#include "Scene.h"

using namespace rujin;

void Tron::Load()
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
