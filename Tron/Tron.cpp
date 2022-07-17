#include "TronPCH.h"
#include "Tron.h"

#include "FMOD_AudioProvider.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "Rujin.h"
#include "SceneManager.h"
#include "ServiceLocator.h"

using namespace rujin;

void Tron::Configure(settings::InitializationParameters& params)
{
	params.windowTitle = "Rujin Demo: Tron";
	params.windowSize = { 750 , 790 };
}

void Tron::Load()
{
	auto& audioService = ServiceLocator::RegisterService<AudioService, FMOD_AudioProvider>();
	auto& resourceManager = ResourceManager::GetInstance();
	/*auto* pScene = */SceneManager::GetInstance().CreateScene("Demo");

	LoadTextures(resourceManager);
	LoadFonts(resourceManager);
	LoadAudio(audioService);
}

void Tron::LoadTextures(rujin::ResourceManager& rm)
{
	//load required textures
	rm.LoadTexture("background.jpg");
	rm.LoadTexture("logo.png");
}

void Tron::LoadFonts(rujin::ResourceManager& rm)
{
	rm.LoadFont("Lingua.otf", 20);
}

void Tron::LoadAudio(rujin::AudioService& /*as*/)
{
}
