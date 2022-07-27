#pragma once
#include "GameObject.h"

namespace rujin
{
	class Camera;

	class Scene final : public IGameLoopObject
	{
	public:
		Scene() = delete;
		~Scene() override;
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

		void Start() override;
		void LateStart() override;
		void Update() override;
		void FixedUpdate() override;
		void OnGui(SDL_Window* pWindow) override;
		void Draw() const override;
		void Destroy() override;

		void AddGameObject(std::unique_ptr<GameObject>& gameObject);

		Camera* GetActiveCamera() const { return m_pActiveCamera; }
		void SetActiveCamera(Camera* pCamera) { m_pActiveCamera = pCamera; }

	private:
		friend class SceneProvider;
		explicit Scene(const std::string& name, Camera* pCamera);

		const std::string m_Name;
		static uint32_t m_IdCounter;

		std::vector<std::unique_ptr<GameObject>> m_GameObjects;
		std::vector<std::weak_ptr<GameObject>> m_ActiveGameObjects;
		std::vector<std::weak_ptr<GameObject>> m_InactiveGameObjects;

		Camera* m_pActiveCamera;
		Camera* m_pDefaultCamera = nullptr;
	};

}
