#pragma once
#include "Singleton.h"
#include "Scene.h"

namespace rujin
{
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		~SceneManager() override;
		SceneManager(const SceneManager& other) = delete;
		SceneManager(SceneManager&& other) noexcept = delete;
		SceneManager& operator=(const SceneManager& other) = delete;
		SceneManager& operator=(SceneManager&& other) noexcept = delete;

		void Start();
		void Update();
		void FixedUpdate();
		void OnGui(SDL_Window* pWindow);
		void Render() const;

		Scene* CreateScene(const std::string& name);
		Scene* GetScene(const std::string& name) const;
		Scene* GetScene(const size_t idx = 0) const;

	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;

		std::vector<std::unique_ptr<Scene>> m_Scenes;
	};
}
