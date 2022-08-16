#ifndef GRAPH_CONNECTION_TYPES_H
#define GRAPH_CONNECTION_TYPES_H


#include "GraphEnums.h"
#include <glm/common.hpp>

namespace rujin::graph
{
	class GraphConnection
	{
	public:
		explicit GraphConnection(int from = invalid_node_index, int to = invalid_node_index, float cost = 1.f);
		virtual ~GraphConnection() = default;

		int GetFrom() const { return m_From; }
		void SetFrom(int newFrom) { m_From = newFrom; }

		int GetTo() const { return m_To; }
		void SetTo(int newTo) { m_To = newTo; }

		float GetCost() const { return m_Cost; }
		void SetCost(float newCost) { m_Cost = newCost; }

		bool IsValid() const { return (m_From != -1 && m_To != -1); }

		bool operator==(const GraphConnection& rhs) const;
		bool operator!=(const GraphConnection& rhs) const;

	protected:
		int m_From;
		int m_To;

		// the cost of traversing the edge
		float m_Cost;
	};

	class GraphConnection2D : public GraphConnection
	{
	public:
		GraphConnection2D(int from = invalid_node_index, int to = invalid_node_index, float cost = 1.f);
		~GraphConnection2D() override = default;

		const glm::vec4& GetColor() const { return m_Color; }
		void SetColor(const glm::vec4& color) { m_Color = color; }

	protected:
		glm::vec4 m_Color;
	};
}

#endif // Include Guard: GRAPH_CONNECTION_TYPES_H
