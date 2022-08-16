#ifndef SCENE_H
#define SCENE_H

#include "AStar.h"
#include "GameObject.h"

namespace rujin
{
	class CollisionQuadTree;
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
		void ProcessAdditionsAndRemovals();
		void OnGui(SDL_Window* pWindow) override;
		void Draw() const override;

		void AddGameObject(GameObject* gameObject);
		void RemoveGameObject(GameObject* gameObject);

		GameObject* GetRootGameObjectByPredicate(const std::function<bool(GameObject*)>& predicate);
		std::vector<GameObject*> GetAllRootGameObjectsByPredicate(const std::function<bool(GameObject*)>& predicate);

		void SetPathfinder(Pathfinder<graph::GridTerrainNode, graph::GraphConnection>* pPathfinder);
		Pathfinder<graph::GridTerrainNode, graph::GraphConnection>* GetPathfinder();

		DeferredVector<GameObject*, GameObject*>& GetEnabledGameObjects();
		DeferredVector<GameObject*, GameObject*>& GetDisabledGameObjects();

		Camera* GetActiveCamera() const { return m_pActiveCamera; }
		void SetActiveCamera(Camera* pCamera) { m_pActiveCamera = pCamera; }

		CollisionQuadTree* GetCollisionQuadTree() const;

		bool HasStarted() const;
	private:
		friend class SceneProvider;
		explicit Scene(const std::string& name, const Rectf& collisionTreeBounds, Camera* pCamera);

		const std::string m_Name;
		static uint32_t m_IdCounter;

		std::unique_ptr<CollisionQuadTree> m_pCollisionQuadTree;

		DeferredVector<GameObject*, GameObject*> m_ActiveGameObjects{};
		DeferredVector<GameObject*, GameObject*> m_InactiveGameObjects{};
		DeferredVector<std::unique_ptr<GameObject>, GameObject*> m_GameObjects{};

		Camera* m_pActiveCamera;
		Camera* m_pDefaultCamera = nullptr;

		Pathfinder<graph::GridTerrainNode, graph::GraphConnection>* m_pPathfinder;

		bool m_IsStarted = false;

	};

}


#endif // Include Guard: SCENE_H
