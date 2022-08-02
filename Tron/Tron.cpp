#include "TronPCH.h"
#include "Tron.h"

#include <SDL_image.h>

#include "BoxCollider.h"
#include "BoxColliderComponent.h"
#include "Camera.h"
#include "CollisionTestComponent.h"
#include "FMOD_AudioProvider.h"
#include "GameObject.h"
#include "InputService.h"
#include "ResourceProvider.h"
#include "Rujin.h"
#include "Scene.h"
#include "SceneProvider.h"
#include "ServiceLocator.h"
#include "TextureRenderComponent.h"
#include "TronPlayerComponent.h"
#include "TronTestComponent.h"

using namespace rujin;

void Tron::Configure(settings::InitParams& params)
{
	params.windowTitle = "Rujin Demo: Tron";
	params.windowSize = { 750 , 790 };
}

void Tron::Load()
{
	AudioService& audioService = ServiceLocator::GetService<AudioService>();
	ResourceService& resourceService = ServiceLocator::GetService<ResourceService>();
	SceneService& sceneService = ServiceLocator::GetService<SceneService>();

	LoadTextures(resourceService);
	LoadFonts(resourceService);
	LoadAudio(audioService);

	const auto& windowSize = Rujin::Get()->GetWindowContext().windowSize;
	Scene* pScene = sceneService.CreateScene("Tron Demo Scene", Rectf{0, 0, static_cast<float>(windowSize.x), static_cast<float>(windowSize.y)});

	{
		auto go = std::make_unique<GameObject>("TestObject2");
		go->AddComponent(new TextureRenderComponent(resourceService.LoadTexture("Textures/Circuit.png"), {0.f, 0.f}));
		pScene->AddGameObject(go);
	}

	SDL_Surface* pLevelTex = IMG_Load("../Data/Levels/Level1.png");
	if (!pLevelTex)
		LOG_ERROR_("Error trying to load level layout file: {}", SDL_GetError());

	const glm::vec2 gridStart{15, 16};
	const float cellSize = 25.f;
	const glm::ivec2 gridDimensions{ 29, 26 };

	if (pLevelTex->w * pLevelTex->h != gridDimensions.x * gridDimensions.y)
		LOG_ERROR_("Level texture size wrong. Level texture must be {}x{}px!", gridDimensions.x, gridDimensions.y);

	//loop over the grid and create GameObjects.
	for (uint8_t y = 0; y < gridDimensions.y; ++y)
	{
		for (uint8_t x = 0; x < gridDimensions.x; ++x)
		{
			//calculated red color of target pixel (ptr arithmetic: +1 = green, +1 = blue)  
			Uint8 pixelColor =
				*(
					(Uint8*)pLevelTex->pixels + 
					pLevelTex->pitch + (pLevelTex->h - 2) * pLevelTex->pitch - (y * pLevelTex->pitch) + 
					x * pLevelTex->format->BytesPerPixel
				);

			if (pixelColor == 255)
			{
				//if the pixel is white spawn a road at that grid cell.

				auto go = std::make_unique<GameObject>(fmt::format("Grid-{}_{}", x, y));

				//Render floor texture
				go->AddComponent(new TextureRenderComponent(resourceService.LoadTexture("Textures/Floor.png"), { 0.f, 0.f }));
				go->AddComponent(new BoxColliderComponent({ 25, 25 }));

				//calc position
				const Position pos{ gridStart.x + x * cellSize, gridStart.y + y * cellSize };
				go->GetTransform()->SetPosition(pos);

				pScene->AddGameObject(go);
			}
		}
	}
	SDL_FreeSurface(pLevelTex);

	{
		auto go = std::make_unique<GameObject>("PlayerTest");

		//Render floor texture
		go->AddComponent(new TextureRenderComponent(resourceService.LoadTexture("Textures/Tank.png"), { 0.f, 0.f }));
		go->AddComponent(new BoxColliderComponent({ 50, 50 }, false));
		go->AddComponent<TronPlayerComponent>();

		//calc position
		go->GetTransform()->SetPosition({50, 700});

		pScene->AddGameObject(go);
	}
}

void Tron::LoadTextures(ResourceService& rs)
{
	//load required textures
	rs.LoadTexture("Textures/Circuit.png");
	rs.LoadTexture("Textures/Floor.png");
}

void Tron::LoadFonts(ResourceService& rs)
{
	rs.LoadFont("Lingua.otf", 20);
}

void Tron::LoadAudio(AudioService& /*as*/)
{
}