#ifndef GRID_GRAPH_H
#define GRID_GRAPH_H

#include "IGraph.h"
#include "GraphConnectionTypes.h"
#include "GraphNodeTypes.h"

#include <vector>

namespace rujin::graph
{

	template<class T_NodeType, class T_ConnectionType>
	class GridGraph : public IGraph<T_NodeType, T_ConnectionType>
	{
	public:
		GridGraph(bool isDirectional);
		GridGraph(const glm::vec2& startPos,int columns, int rows, float cellSize, bool isDirectionalGraph, bool isConnectedDiagonally, float costStraight = 1.f, float costDiagonal = 1.5);
		void InitializeGrid(const glm::vec2& startPos, int columns, int rows, float cellSize, bool isDirectionalGraph, bool isConnectedDiagonally, float costStraight = 1.f, float costDiagonal = 1.5);

		using IGraph<T_NodeType, T_ConnectionType>::GetNode;
		T_NodeType* GetNode(int col, int row) const { return this->m_Nodes[GetIndex(col, row)]; }
		const std::list<T_ConnectionType*>& GetConnections(const T_NodeType& node) const { return this->m_Connections[node.GetIndex()]; }
		const std::list<T_ConnectionType*>& GetConnections(int idx) const { return this->m_Connections[idx]; }

		int GetRows() const { return m_NrOfRows; }
		int GetColumns() const { return m_NrOfColumns; }

		bool IsWithinBounds(int col, int row) const;
		int GetIndex(int col, int row) const { return row * m_NrOfColumns + col; }

		// returns the column and row of the node in a glm::vec2
		using IGraph<T_NodeType, T_ConnectionType>::GetNodePos;
		virtual glm::vec2 GetNodePos(T_NodeType* pNode) const override;

		// returns the actual world position of the node
		using IGraph<T_NodeType, T_ConnectionType>::GetNodeWorldPos;
		glm::vec2 GetNodeWorldPos(int col, int row) const;
		glm::vec2 GetNodeWorldPos(int idx) const override;

		int GetNodeIdxAtWorldPos(const glm::vec2& pos) const override;

		void AddConnectionsToAdjacentCells(int col, int row);
		void AddConnectionsToAdjacentCells(int idx);
	private:
		glm::vec2 m_StartPos;
		int m_NrOfColumns;
		int m_NrOfRows;
		float m_CellSize;

		bool m_IsConnectedDiagonally;
		float m_DefaultCostStraight;
		float m_DefaultCostDiagonal;

		const std::vector<glm::vec2> m_StraightDirections = { { 1, 0 }, { 0, 1 }, { -1, 0 }, { 0, -1 } };
		const std::vector<glm::vec2> m_DiagonalDirections = { { 1, 1 }, { -1, 1 }, { -1, -1 }, { 1, -1 } };

		// graph creation helper functions
		void AddConnectionsInDirections(int idx, int col, int row, std::vector<glm::vec2> directions);

		float CalculateConnectionCost(int fromIdx, int toIdx) const;

		friend class GraphRenderer;
	};

	template<class T_NodeType, class T_ConnectionType>
	inline GridGraph<T_NodeType, T_ConnectionType>::GridGraph(bool isDirectional)
		: IGraph(isDirectional)
		, m_StartPos{}
		  , m_NrOfColumns(0)
		  , m_NrOfRows(0)
		  , m_CellSize(5)
		  , m_IsConnectedDiagonally(true)
		  , m_DefaultCostStraight(1.f)
		  , m_DefaultCostDiagonal(1.5f)
	{
	}

	template<class T_NodeType, class T_ConnectionType>
	GridGraph<T_NodeType, T_ConnectionType>::GridGraph(
		const glm::vec2& startPos,
		int columns,
		int rows,
		float cellSize,
		bool isDirectionalGraph,
		bool isConnectedDiagonally,
		float costStraight /* = 1.f*/,
		float costDiagonal /* = 1.5f */)
		: IGraph<T_NodeType, T_ConnectionType>(isDirectionalGraph)
		, m_StartPos(startPos)
		, m_NrOfColumns(columns)
		, m_NrOfRows(rows)
		, m_CellSize(cellSize)
		, m_IsConnectedDiagonally(isConnectedDiagonally)
		, m_DefaultCostStraight(costStraight)
		, m_DefaultCostDiagonal(costDiagonal)
	{
		InitializeGrid(startPos, columns, rows, cellSize, isDirectionalGraph, isConnectedDiagonally, costStraight, costDiagonal);
	}

	template<class T_NodeType, class T_ConnectionType>
	inline void GridGraph<T_NodeType, T_ConnectionType>::InitializeGrid(
		const glm::vec2& startPos,
		int columns,
		int rows,
		float cellSize,
		bool isDirectionalGraph,
		bool isConnectedDiagonally,
		float costStraight /* = 1.f*/,
		float costDiagonal /* = 1.5f */)
	{
		m_StartPos = startPos;
		this->m_IsDirectionalGraph = isDirectionalGraph;
		m_NrOfColumns = columns;
		m_NrOfRows = rows;
		m_CellSize = cellSize;
		m_IsConnectedDiagonally = isConnectedDiagonally;
		m_DefaultCostStraight = costStraight;
		m_DefaultCostDiagonal = costDiagonal;

		// Create all nodes
		for (auto r = 0; r < m_NrOfRows; ++r)
		{
			for (auto c = 0; c < m_NrOfColumns; ++c)
			{
				int idx = GetIndex(c, r);
				this->AddNode(new T_NodeType(idx));
			}
		}

		// Create connections in each valid direction on each node
		for (auto r = 0; r < m_NrOfRows; ++r)
		{
			for (auto c = 0; c < m_NrOfColumns; ++c)
			{
				AddConnectionsToAdjacentCells(c, r);
			}
		}
	}

	template<class T_NodeType, class T_ConnectionType>
	bool GridGraph<T_NodeType, T_ConnectionType>::IsWithinBounds(int col, int row) const
	{
		return (col >= 0 && col < m_NrOfColumns&& row >= 0 && row < m_NrOfRows);
	}


	template<class T_NodeType, class T_ConnectionType>
	void GridGraph<T_NodeType, T_ConnectionType>::AddConnectionsToAdjacentCells(int col, int row)
	{
		int idx = GetIndex(col, row);

		// Add connections in all directions, taking into account the dimensions of the grid
		AddConnectionsInDirections(idx, col, row, m_StraightDirections);

		if (m_IsConnectedDiagonally)
		{
			AddConnectionsInDirections(idx, col, row, m_DiagonalDirections);
		}
		
		this->OnGraphModified(false, true);
	}

	template<class T_NodeType, class T_ConnectionType>
	inline void GridGraph<T_NodeType, T_ConnectionType>::AddConnectionsToAdjacentCells(int idx)
	{
		auto colRow = this->GetNodePos(idx);
		AddConnectionsToAdjacentCells((int)colRow.x, (int)colRow.y);
	}

	template<class T_NodeType, class T_ConnectionType>
	void GridGraph<T_NodeType, T_ConnectionType>::AddConnectionsInDirections(int idx, int col, int row, std::vector<glm::vec2> directions)
	{
		for (auto d : directions)
		{
			int neighborCol = col + (int)d.x;
			int neighborRow = row + (int)d.y;

			if (IsWithinBounds(neighborCol, neighborRow))
			{
				int neighborIdx = neighborRow * m_NrOfColumns + neighborCol;
				float connectionCost = CalculateConnectionCost(idx, neighborIdx);

				if (this->IsUniqueConnection(idx, neighborIdx)
					&& connectionCost < 100000) //Extra check for different terrain types
					this->AddConnection(new GraphConnection(idx, neighborIdx, connectionCost));
			}
		}
	}

	template<class T_NodeType, class T_ConnectionType>
	inline float GridGraph<T_NodeType, T_ConnectionType>::CalculateConnectionCost(int fromIdx, int toIdx) const
	{
		float cost = m_DefaultCostStraight;

		glm::vec2 fromPos = this->GetNodePos(fromIdx);
		glm::vec2 toPos = this->GetNodePos(toIdx);

		if (int(fromPos.y) != int(toPos.y) &&
			int(fromPos.x) != int(toPos.x))
		{
			cost = m_DefaultCostDiagonal;
		}

		return cost;
	}

	template<>
	inline float GridGraph<GridTerrainNode, GraphConnection>::CalculateConnectionCost(int fromIdx, int toIdx) const
	{
		float cost = m_DefaultCostStraight;

		glm::vec2 fromPos = GetNodePos(fromIdx);
		glm::vec2 toPos = GetNodePos(toIdx);

		if (int(fromPos.y) != int(toPos.y) &&
			int(fromPos.x) != int(toPos.x))
		{
			cost = m_DefaultCostDiagonal;
		}

		cost *= (int(GetNode(fromIdx)->GetTerrainType()) + int(this->GetNode(toIdx)->GetTerrainType())) / 2.0f;

		return cost;
	}

	template<class T_NodeType, class T_ConnectionType>
	glm::vec2 GridGraph<T_NodeType, T_ConnectionType>::GetNodePos(T_NodeType* pNode) const
	{
		auto col = pNode->GetIndex() % m_NrOfColumns;
		auto row = pNode->GetIndex() / m_NrOfColumns;

		return glm::vec2{ float(col), float(row) };
	}

	template<class T_NodeType, class T_ConnectionType>
	glm::vec2 GridGraph<T_NodeType, T_ConnectionType>::GetNodeWorldPos(int col, int row) const
	{
		glm::vec2 cellCenterOffset = { m_CellSize / 2.f, m_CellSize / 2.f };
		return m_StartPos + glm::vec2{ (float)col * m_CellSize, (float)row * m_CellSize } + cellCenterOffset;
	}

	template<class T_NodeType, class T_ConnectionType>
	glm::vec2 GridGraph<T_NodeType, T_ConnectionType>::GetNodeWorldPos(int idx) const
	{
		auto colRow = this->GetNodePos(idx);
		return this->GetNodeWorldPos((int)colRow.x, (int)colRow.y);
	}

	template<class T_NodeType, class T_ConnectionType>
	inline int GridGraph<T_NodeType, T_ConnectionType>::GetNodeIdxAtWorldPos(const glm::vec2& pos) const
	{
		int idx = invalid_node_index;

		//Added extra check since  c = int(pos.x / m_CellSize); => doesnt work correcly when out of the lower bounds
		if (pos.x < 0 || pos.y < 0)
		{
			return idx;
		}

		int r, c;

		c = int((pos.x - m_StartPos.x) / m_CellSize);
		r = int((pos.y - m_StartPos.y) / m_CellSize);

		if (!IsWithinBounds(c, r))
			return idx;

		return GetIndex(c, r);
	}
}


#endif // Include Guard: GRID_GRAPH_H
