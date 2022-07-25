#pragma once
#include "Singleton.h"

namespace rujin
{
	class Camera;

	namespace settings
	{
		struct InitParams;
	}

	class Scene;

	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		SceneManager(const SceneManager& other) = delete;
		SceneManager(SceneManager&& other) noexcept = delete;
		SceneManager& operator=(const SceneManager& other) = delete;
		SceneManager& operator=(SceneManager&& other) noexcept = delete;

		void Start();
		void LateStart();
		void Update();
		void FixedUpdate();
		void OnGui(SDL_Window* pWindow);
		void Draw() const;

		Scene* CreateScene(const std::string& name, Camera* pCamera = nullptr);
		Scene* GetScene(const std::string& name) const;
		Scene* GetScene(const size_t idx = 0) const;

	private:
		friend class Singleton<SceneManager>;
		SceneManager();
		~SceneManager() override;
		void Initialize() override;

		std::vector<std::unique_ptr<Scene>> m_Scenes;
	};
}
