#ifndef TRON_H
#define TRON_H

#include "AStar.h"
#include "GridGraph.h"
#include "IGame.h"

#pragma region Forward Declarations
namespace rujin
{
	class Scene;
	class ResourceService;
	class AudioService;
}

#pragma endregion


class Tron final : public IGame
{
public:
	explicit Tron() = default;
	~Tron() override;

	Tron(const Tron&) = delete;
	Tron(Tron&&) noexcept = delete;
	Tron& operator=(const Tron&) = delete;
	Tron& operator=(Tron&&) noexcept = delete;

	void Configure(settings::InitParams& params) override;
	void Load() override;

	const glm::vec2& GetRandomSpawnLocation() const;

private:
	void GenerateLevelGridFromTexture(Scene* pScene, const std::string& levelTexturePath);
	void CreateLevelBoundsColliders(Scene* pScene);
	void GenerateSpawnLocations();

	/*Level Grid*/
	const glm::vec2 m_GridStart{ 15, 16 };
	const glm::ivec2 m_GridDimensions{ 29, 26 };
	const float m_CellSize = 25.f;
	std::vector<glm::vec2> m_SpawnLocations{};

	const float m_BoundsColliderSize{ 30.f };

	/* Pathfinding */
	graph::GridGraph<graph::GridTerrainNode, graph::GraphConnection>* m_pGridGraph;
	AStar<graph::GridTerrainNode, graph::GraphConnection>* m_pAStar;
};


#endif // Include Guard: TRON_H
