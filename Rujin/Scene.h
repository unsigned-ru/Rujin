#pragma once
#include "GameObject.h"
#include "MonoBehaviour.h"
#include "SceneManager.h"

namespace rujin
{
	class Scene : public MonoBehaviour
	{
	public:
		Scene() = delete;
		~Scene() override;
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

		void Start() override;
		void Update() override;
		void FixedUpdate() override;
		void Render() const override;
		void Destroy() override;

		std::weak_ptr<GameObject> AddGameObject(const std::shared_ptr<GameObject> gameObject);

	private:
		friend class SceneManager;
		explicit Scene(const std::string& name);

	protected:
		const std::string m_Name;
		static uint32_t m_IdCounter;

		std::vector<std::shared_ptr<GameObject>> m_GameObjects;
		std::vector<std::weak_ptr<GameObject>> m_ActiveGameObjects;
		std::vector<std::weak_ptr<GameObject>> m_InactiveGameObjects;
	};

}
