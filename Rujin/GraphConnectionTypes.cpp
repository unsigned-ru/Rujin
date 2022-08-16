#include "RujinPCH.h"
#include "GraphConnectionTypes.h"

#include "GraphVisuals.h"

rujin::graph::GraphConnection::GraphConnection(int from /* = invalid_node_index*/, int to /* = invalid_node_index*/, float cost /* = 1.f*/)
	: m_From(from)
	, m_To(to)
	, m_Cost(cost)
{
}

bool rujin::graph::GraphConnection::operator==(const GraphConnection& rhs) const
{
	return rhs.m_From == this->m_From &&
		rhs.m_To == this->m_To &&
		rhs.m_Cost == this->m_Cost;
}

bool rujin::graph::GraphConnection::operator!=(const GraphConnection& rhs) const
{
	return !(*this == rhs);
}

// ---------------------------------------------------------------------------------------------------------------------


rujin::graph::GraphConnection2D::GraphConnection2D(int from /* = invalid_node_index*/, int to /* = invalid_node_index*/, float cost /* = 1.f*/)
	:GraphConnection(from, to, cost)
	, m_Color(DEFAULT_CONNECTION_COLOR)
{

}