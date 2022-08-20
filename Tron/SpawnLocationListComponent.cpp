#include "TronPCH.h"
#include "SpawnLocationListComponent.h"

#include "GameObject.h"
#include "Scene.h"
#include "Tron.h"

void SpawnLocationListComponent::LateStart()
{
	Component::LateStart();

	if (m_SpawnLocations.empty())
		GenerateSpawnLocations(GameObject()->GetScene()->GetPathfinder()->GetGraph());
}

const glm::vec2& SpawnLocationListComponent::GetRandomSpawnLocation() const
{
	static std::uniform_int_distribution<uint16_t> dist(0u, static_cast<uint16_t>(m_SpawnLocations.size()) - 1u);
	const uint16_t idx = dist(Rujin::Get()->GetRandomEngine());
	return m_SpawnLocations.at(idx);
}

void SpawnLocationListComponent::GenerateSpawnLocations(graph::GridGraph<graph::GridTerrainNode, graph::GraphConnection>* pGraph)
{
	const auto& gridDimensions = Rujin::Get()->GetGame<Tron>()->GetGridDimensions();
	const auto& gridStart = pGraph->GetGridStart();
	const auto gridCellSize = pGraph->GetCellSize();

	for (uint8_t y = 0; y < gridDimensions.y - 2; ++y)
	{
		for (uint8_t x = 0; x < gridDimensions.x - 2; ++x)
		{
			//if this, right, top, and topright cells aren't walls... then it is a spawn location!
			if
				(
					pGraph->GetNode(x, y)->GetTerrainType() == graph::TerrainType::Road &&			//this
					pGraph->GetNode(x + 1, y)->GetTerrainType() == graph::TerrainType::Road &&		//right
					pGraph->GetNode(x, y + 1)->GetTerrainType() == graph::TerrainType::Road &&		//top
					pGraph->GetNode(x + 1, y + 1)->GetTerrainType() == graph::TerrainType::Road	//top-right
				)
			{
				//add the middle of the 4 cells as spawn position
				m_SpawnLocations.emplace_back(gridStart.x + (x + 1) * gridCellSize, gridStart.y + (y + 1) * gridCellSize);
			}
		}
	}
}
