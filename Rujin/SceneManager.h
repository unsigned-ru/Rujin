#pragma once
#include "Singleton.h"
#include <unordered_map>

#include "MonoBehaviour.h"

namespace rujin
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		void Start();
		void Update();
		void FixedUpdate();
		void Render() const;

		std::weak_ptr<Scene> CreateScene(const std::string& name);
		std::weak_ptr<Scene> GetScene(const std::string& name) const;
		std::weak_ptr<Scene> GetScene(const size_t idx = 0) const;

	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;

		//All copies should be weak pointers!
		std::vector<std::shared_ptr<Scene>> m_Scenes;
	};
}
