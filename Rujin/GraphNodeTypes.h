#pragma once
#include "GraphEnums.h"
#include "GraphVisuals.h"
#include <glm/common.hpp>

namespace rujin::graph
{
	class GraphNode
	{
	public:
		GraphNode() : m_Index(invalid_node_index) {}
		explicit GraphNode(int idx) : m_Index(idx) {}

		virtual ~GraphNode() = default;

		int GetIndex() const { return m_Index; }
		void SetIndex(int newIdx) { m_Index = newIdx; }

		bool operator==(const GraphNode& rhs) { return m_Index == rhs.m_Index; }

	protected:
		int m_Index;
	};

	class GraphNode2D : public GraphNode
	{
	public:
		GraphNode2D(int index, const glm::vec2& pos = glm::vec2{})
			: GraphNode(index), m_Position(pos), m_Color(DEFAULT_NODE_COLOR)
		{
		}
		virtual ~GraphNode2D() = default;

		glm::vec2 GetPosition() const { return m_Position; }
		void SetPosition(const glm::vec2& newPos) { m_Position = newPos; }

		glm::vec4 GetColor() const { return m_Color; }
		void SetColor(const glm::vec4& color) { m_Color = color; }

	protected:
		glm::vec2 m_Position;
		glm::vec4 m_Color;
	};


	class GridTerrainNode : public GraphNode
	{
	public:
		GridTerrainNode(int index)
			: GraphNode(index), m_Terrain(TerrainType::Road)
		{
		}
		virtual ~GridTerrainNode() = default;


		TerrainType GetTerrainType() const { return m_Terrain; }
		void SetTerrainType(TerrainType terrain) { m_Terrain = terrain; }
		glm::vec4 GetColor() const
		{
			switch (m_Terrain)
			{
			case TerrainType::Wall:
				return WALL_NODE_COLOR;
				break;
			default:
				return ROAD_NODE_COLOR;
				break;
			}
		}


	protected:
		TerrainType m_Terrain;
	};


	class NavGraphNode : public GraphNode2D
	{
	public:
		NavGraphNode(int index, const glm::vec2& pos = {})
			: GraphNode2D(index, pos), m_LineIdx(0) {}
		NavGraphNode(int index, int lineIdx, const glm::vec2& pos = {})
			: GraphNode2D(index, pos), m_LineIdx(lineIdx) {}
		~NavGraphNode() override = default;
		int GetLineIndex() const { return m_LineIdx; };
	protected:
		int m_LineIdx;
	};

	class InfluenceNode final : public GraphNode2D
	{
	public:
		InfluenceNode(int index, glm::vec2 pos = { 0,0 }, float influence = 0.f)
			: GraphNode2D(index, pos), m_Influence(influence) {}

		float GetInfluence() const { return m_Influence; }
		void SetInfluence(float influence) { m_Influence = influence; }

	private:
		float m_Influence;
	};
}
