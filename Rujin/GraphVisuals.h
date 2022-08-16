#ifndef GRAPH_VISUALS_H
#define GRAPH_VISUALS_H

#include <glm/common.hpp>

namespace rujin::graph
{
	//Colors
	const glm::vec4 DEFAULT_NODE_COLOR{ 0.7f, 0.7f, 0.7f, 1.f };
	const glm::vec4 DEFAULT_CONNECTION_COLOR{ .2f, .2f, .2f, 1.f };
	
	const glm::vec4 HIGHLIGHTED_NODE_COLOR{ 0.f, 0.8f, 0.1f, 1.f };
	const glm::vec4 START_NODE_COLOR{ 0.f, 1.0f, 0.0f, 1.f };
	const glm::vec4 END_NODE_COLOR{ 1.f, 0.f, 0.f, 1.f };

	const glm::vec4 ROAD_NODE_COLOR{ DEFAULT_NODE_COLOR };
	const glm::vec4 WALL_NODE_COLOR{ 0.5f, 0.9f, 0.9f, 1.f };


	//Radius
	constexpr float DEFAULT_NODE_RADIUS{ 3.f };
}


#endif // Include Guard: GRAPH_VISUALS_H
