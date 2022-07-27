#pragma once
#include "Scene.h"
#include "Service.h"

struct SDL_Window;

namespace rujin
{
	class SceneService : public Service
	{
	public:
		SceneService() = default;
		~SceneService() override = default;

		SceneService(const SceneService&) = delete;
		SceneService(SceneService&&) noexcept = delete;
		SceneService& operator=(const SceneService&) = delete;
		SceneService& operator=(SceneService&&) noexcept = delete;

		virtual void Start() = 0;
		virtual void LateStart() = 0;
		virtual void Update() = 0;
		virtual void FixedUpdate() = 0;
		virtual void OnGui(SDL_Window* pWindow) = 0;
		virtual void Draw() const = 0;

		virtual Scene* CreateScene(const std::string& name, Camera* pCamera = nullptr) = 0;
		virtual Scene* GetScene(const std::string& name) const = 0;
		virtual Scene* GetScene(const size_t idx = 0) const = 0;
	};
}
