#ifndef SPAWN_LOCATION_LIST_COMPONENT_H
#define SPAWN_LOCATION_LIST_COMPONENT_H

#include "Component.h"
#include "GridGraph.h"

class SpawnLocationListComponent : public Component
{
public:
	SpawnLocationListComponent() = default;

	void LateStart() override;

	void GenerateSpawnLocations(graph::GridGraph<graph::GridTerrainNode, graph::GraphConnection>* pGraph);
	const glm::vec2& GetRandomSpawnLocation() const;
private:
	std::vector<glm::vec2> m_SpawnLocations{};
};


#endif // Include Guard: SPAWN_LOCATION_LIST_COMPONENT_H
