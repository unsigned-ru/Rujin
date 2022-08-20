#ifndef ASTAR_H
#define ASTAR_H

#include "GridGraph.h"
typedef float(*Heuristic)(float, float);

namespace rujin
{
	template <typename T_NodeType, typename T_ConnectionType>
	class Pathfinder
	{
	public:
		Pathfinder() = default;

		virtual std::vector<T_NodeType*> FindPath(const glm::vec2& startPos, const glm::vec2& destination) = 0;
		virtual glm::vec2 GetMovementDir(const glm::vec2& startPos, const glm::vec2& destination, uint32_t pathIdx= 0u) = 0;

		virtual graph::GridGraph<T_NodeType, T_ConnectionType>* GetGraph() const = 0;

		virtual ~Pathfinder() = default;

		Pathfinder(const Pathfinder&) = delete;
		Pathfinder(Pathfinder&&) noexcept = delete;
		Pathfinder& operator=(const Pathfinder&) = delete;
		Pathfinder& operator=(Pathfinder&&) noexcept = delete;
	};

	template <typename T_NodeType, typename T_ConnectionType>
	class AStar final : public Pathfinder<T_NodeType, T_ConnectionType>
	{
	public:
		AStar(graph::GridGraph<T_NodeType, T_ConnectionType>* pGraph, Heuristic hFunction);
		~AStar() override
		{
			delete m_pGraph;
		};

		AStar(const AStar&) = delete;
		AStar(AStar&&) noexcept = delete;
		AStar& operator=(const AStar&) = delete;
		AStar& operator=(AStar&&) noexcept = delete;

		graph::GridGraph<T_NodeType, T_ConnectionType>* GetGraph() const override
		{
			return m_pGraph;
		}

		// stores the optimal connection to a node and its total costs related to the start and end node of the path
		struct NodeRecord
		{
			T_NodeType* pNode = nullptr;
			T_ConnectionType* pConnection = nullptr;
			float costSoFar = 0.f; // accumulated g-costs of all the connections leading up to this one
			float estimatedTotalCost = 0.f; // f-cost (= costSoFar + h-cost)

			bool operator==(const NodeRecord& other) const
			{
				return pNode == other.pNode
					&& pConnection == other.pConnection
					&& costSoFar == other.costSoFar
					&& estimatedTotalCost == other.estimatedTotalCost;
			}

			bool operator<(const NodeRecord& other) const
			{
				return estimatedTotalCost < other.estimatedTotalCost;
			}
		};

		glm::vec2 GetMovementDir(const glm::vec2& startPos, const glm::vec2& destination, uint32_t pathIdx = 0u) override
		{
			std::vector<T_NodeType*> path = FindPath(startPos, destination);

			if (path.empty() || pathIdx > path.size() - 1)
				return glm::vec2{};

			return glm::normalize(m_pGraph->GetNodeWorldPos(path[pathIdx]) - startPos);
		}

		std::vector<T_NodeType*> FindPath(const glm::vec2& startPos, const glm::vec2& destination) override
		{
			//Final path
			std::vector<T_NodeType*> path{};

			//Connections to be checked 
			std::vector<NodeRecord> openList{};

			//Connections already checked
			std::vector<NodeRecord> closedList{};

			T_NodeType* pStartNode = m_pGraph->GetNodeAtWorldPos(startPos);
			T_NodeType* pDestinationNode = m_pGraph->GetNodeAtWorldPos(destination);

			if (!pStartNode || !pDestinationNode)
				return {}; // if eiher the starting or ending node is outside of the pathfinding grid, then return no path...

			NodeRecord currentRecord{};
			currentRecord.pNode = pStartNode;
;			currentRecord.estimatedTotalCost = GetHeuristicCost(currentRecord.pNode, pDestinationNode);
			openList.push_back(currentRecord);

 			while (true) //break statement handled at the end of loop body. Not an infinite loop.
			{
				//Get node with lowest HeuristicCost
				currentRecord = *std::min_element(openList.begin(), openList.end());

				//check if the connection leads to the end node.
				if (currentRecord.pNode == pDestinationNode)
					break;

				// get all connections of the cheapest record
				for (T_ConnectionType* connection : m_pGraph->GetNodeConnections(currentRecord.pNode))
				{
					//calculate total cost
					float connectionCostSoFar{ currentRecord.costSoFar + connection->GetCost() };

					T_NodeType* nodeTo{ m_pGraph->GetNode(connection->GetTo()) };

					// check if connection leads to a node that's already on the closed list
					auto closedIt = std::find_if(closedList.begin(), closedList.end(), [nodeTo](NodeRecord& record) { return record.pNode == nodeTo; });
					if (closedIt != closedList.end())
					{
						//if the old connection is cheaper
						if (closedIt->costSoFar <= connectionCostSoFar)
							continue; //to the next connection
						
						//remove it from the closed list (so it can be replaced)
						closedList.erase(closedIt);
					}

					// check if connection leads to a node that's already on the open list
					auto openIt = std::find_if(openList.begin(), openList.end(), [nodeTo](NodeRecord& record) { return record.pNode == nodeTo; });
					if (openIt != openList.end())
					{
						//if the old connection is cheaper
						if (openIt->costSoFar <= connectionCostSoFar)
							continue; //to the next connection
						
						//remove it from the open list (so it can be replaced)
						openList.erase(openIt);
					}

					//At this point any expensive connection should be removed (if it existed)
					//Create a new NodeRecord and add it to the openList.
					NodeRecord record{};
					record.pNode = nodeTo;
					record.costSoFar = connectionCostSoFar;
					record.pConnection = connection;
					record.estimatedTotalCost = connectionCostSoFar + GetHeuristicCost(record.pNode, pDestinationNode);

					openList.push_back(record);
				}

				// remove current record from openList
				auto it = std::find(openList.begin(), openList.end(), currentRecord);
				if (it != openList.end())
					openList.erase(it);

				//add current record to the closedList
				closedList.push_back(currentRecord);

				if (openList.empty())
					break;
				
				currentRecord = *(openList.end() - 1);
			}

			//reconstruct path from last connection to start node
			//Track back from the currentRecord until the node of the record is the startnode of the overall path
			while (currentRecord.pNode != pStartNode)
			{
				//add current record's node.
				path.push_back(currentRecord.pNode);

				//Look in the closedList for a record where pNode == the currentRecords’ connections’ startNode
				auto previousConnectionStartNode{ m_pGraph->GetNode(currentRecord.pConnection->GetFrom()) };

				currentRecord = *std::find_if(closedList.begin(), closedList.end(), [previousConnectionStartNode](NodeRecord& record) { return record.pNode == previousConnectionStartNode; });
			}

			//add the start node.
			path.push_back(currentRecord.pNode);

			//reverse path
			std::reverse(path.begin(), path.end());

			return path;
		}

	private:
		float GetHeuristicCost(T_NodeType* pStartNode, T_NodeType* pEndNode) const;
	private:
		graph::GridGraph<T_NodeType, T_ConnectionType>* m_pGraph;
		Heuristic m_HeuristicFunction;
	};

	template <class T_NodeType, class T_ConnectionType>
	AStar<T_NodeType, T_ConnectionType>::AStar(graph::GridGraph<T_NodeType, T_ConnectionType>* pGraph, Heuristic hFunction)
		: m_pGraph(pGraph)
		, m_HeuristicFunction(hFunction)
	{
	}


	template <class T_NodeType, class T_ConnectionType>
	inline float AStar<T_NodeType, T_ConnectionType>::GetHeuristicCost(T_NodeType* pStartNode, T_NodeType* pEndNode) const
	{
		const glm::vec2 toDestination = m_pGraph->GetNodePos(pEndNode) - m_pGraph->GetNodePos(pStartNode);
		return m_HeuristicFunction(abs(toDestination.x), abs(toDestination.y));
	}
}


#endif // Include Guard: ASTAR_H
