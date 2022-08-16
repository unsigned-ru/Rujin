#ifndef GRAPH_ENUMS_H
#define GRAPH_ENUMS_H


namespace rujin::graph
{
	enum
	{
		invalid_node_index = -1
	};

	enum class TerrainType : int
	{
		Road = 1,
		// Node's with a value of over 200 000 are always isolated
		Wall = 200001
	};
}


#endif // Include Guard: GRAPH_ENUMS_H
