#include "TronPCH.h"
#include "Tron.h"

#include "Camera.h"
#include "FMOD_AudioProvider.h"
#include "GameObject.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "Rujin.h"
#include "Scene.h"
#include "SceneManager.h"
#include "ServiceLocator.h"
#include "TextureRenderComponent.h"
#include "TronTestComponent.h"

using namespace rujin;

void Tron::Configure(settings::InitParams& params)
{
	params.windowTitle = "Rujin Demo: Tron";
	params.windowSize = { 750 , 790 };
}

void Tron::Load()
{
	auto& audioService = ServiceLocator::RegisterService<AudioService, FMOD_AudioProvider>();
	auto* resourceManager = ResourceManager::Get();
	auto* pScene = SceneManager::Get()->CreateScene("Demo");

	LoadTextures(resourceManager);
	LoadFonts(resourceManager);
	LoadAudio(audioService);

	//TODO: fix render order.

	{
		auto go = std::make_unique<GameObject>("TestObject2");
		go->AddComponent(new TextureRenderComponent(resourceManager->LoadTexture("background.jpg"), {0.5f, 0.f}));
		go->GetTransform()->SetPosition(Position{ 750 / 2, 0 });
		pScene->AddGameObject(go);
	}

	{
		auto go = std::make_unique<GameObject>("TestObject");
		go->GetTransform()->SetPosition(Position{ 750 / 2, 100 });

		auto go2 = std::make_unique<GameObject>("TestChildObject");
		go2->AddComponent(new TextureRenderComponent(resourceManager->LoadTexture("logo.png")));
		go2->AddComponent<TronTestComponent>();

		go->AddChild(go2);

		pScene->AddGameObject(go);
	}

}

void Tron::LoadTextures(rujin::ResourceManager* rm)
{
	//load required textures
	rm->LoadTexture("background.jpg");
	rm->LoadTexture("logo.png");
}

void Tron::LoadFonts(rujin::ResourceManager* rm)
{
	rm->LoadFont("Lingua.otf", 20);
}

void Tron::LoadAudio(rujin::AudioService& /*as*/)
{
}

void Tron::CreateTestObject()
{

}