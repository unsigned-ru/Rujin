#pragma once
#include "GameObject.h"
#include "IGameLoopObject.h"

namespace rujin
{
	class Scene : public IGameLoopObject
	{
	public:
		Scene() = delete;
		~Scene() override = default;
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

		void Start() override;
		void Update() override;
		void FixedUpdate() override;
		void OnGui(SDL_Window* pWindow) override;
		void Render() const override;
		void Destroy() override;

		void AddGameObject(GameObject* gameObject);

	private:
		friend class SceneManager;
		explicit Scene(const std::string& name);

	protected:
		const std::string m_Name;
		static uint32_t m_IdCounter;

		std::vector<std::unique_ptr<GameObject>> m_GameObjects;
		std::vector<std::weak_ptr<GameObject>> m_ActiveGameObjects;
		std::vector<std::weak_ptr<GameObject>> m_InactiveGameObjects;
	};

}
