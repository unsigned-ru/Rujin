#pragma once
#include "SceneService.h"
#include "Singleton.h"

namespace rujin
{
	class Camera;

	namespace settings
	{
		struct InitParams;
	}

	class Scene;

	class SceneProvider final : public SceneService
	{
	public:
		SceneProvider();
		~SceneProvider() override;

		SceneProvider(const SceneProvider& other) = delete;
		SceneProvider(SceneProvider&& other) noexcept = delete;
		SceneProvider& operator=(const SceneProvider& other) = delete;
		SceneProvider& operator=(SceneProvider&& other) noexcept = delete;

		void Start() override;
		void LateStart() override;
		void Update() override;
		void FixedUpdate() override;
		void OnGui(SDL_Window* pWindow) override;
		void Draw() const override;

		Scene* CreateScene(const std::string& name, Camera* pCamera = nullptr) override;
		Scene* GetScene(const std::string& name) const override;
		Scene* GetScene(const size_t idx = 0) const override;

	private:
		std::vector<std::unique_ptr<Scene>> m_Scenes;
	};
}
