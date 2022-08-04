#ifndef COLLISION_QUAD_TREE_H
#define COLLISION_QUAD_TREE_H
#include "IObserver.h"
#include "Structs.h"

namespace rujin
{
	class CollisionQuadTree final : public event::IObserver
	{
	public:
		CollisionQuadTree(const Rectf& bounds);
		CollisionQuadTree(uint32_t maxObjects, uint32_t maxLevels, uint32_t level, const Rectf& bounds, CollisionQuadTree* pParent);

		// Inserts pCollider into our quadtree.
		Collider* Insert(Collider* pCollider);

		// Removes pCollider from our quadtree when we no longer need it to collide.
		void Remove(Collider* pCollider);

		// Returns vector of colliders that intersect with the search area.
		std::vector<const Collider*> Search(const Rectf& area);

		// Returns the bounds of this node.
		const Rectf& GetBounds() const;

		//Handles collision checking for all registered colliders
		//in this tree and child trees.
		void HandleCollision(CollisionQuadTree* pRoot = nullptr);

		/**
		 * \brief Returns the first hit collider
		 * \param p1 Start point of line.
		 * \param p2 End point of line.
		 * \param pIntesection [OUT] The point where the collider and ray intersected.
		 * \param ppHitCollider [OUT] The collider that was hit.
		 * \return true if any collider was hit, false if none was hit.
		 */
		bool Raycast(const glm::vec2& p1, const glm::vec2& p2, glm::vec2* pIntesection, const Collider** ppHitCollider);

		void DrawDebug() const;

	private:
		//returns all colliders from the cells that overlap the area.
		void Search(const Rectf& area, std::vector<const Collider*>& overlappingColliders);

		//returns all colliders from the cells that the line intersects.
		void Search(const glm::vec2& p1, const glm::vec2& p2, std::vector<const Collider*>& overlappingColliders);

		// Returns the index for the node that will contain 		
		// the object. -1 is returned if it is this node.
		int8_t GetChildIndexForObject(const Rectf& colliderBounds) const;

		// Creates the child nodes.
		void Split();

		void OnNotify(const uint32_t identifier, const event::Data* pEventData) override;

		CollisionQuadTree* GetRoot();

	private:
		std::unique_ptr<CollisionQuadTree> m_Children[4];
		inline static constexpr int8_t s_ChildIdxNE = 0;
		inline static constexpr int8_t s_ChildIdxSE = 1;
		inline static constexpr int8_t s_ChildIdxSW = 2;
		inline static constexpr int8_t s_ChildIdxNW = 3;
		inline static constexpr int8_t s_ThisTree = -1;

		uint32_t m_MaxObjects;
		uint32_t m_MaxLevels;

		CollisionQuadTree* m_pParent;
		
		std::vector<Collider*> m_Colliders;

		// How deep the current node is from the base node. 
		// The first node starts at 0 and then its child node 	
		// is at level 1 and so on.
		uint32_t m_Level;

		// The bounds of this node.
		Rectf m_Bounds;
	};
}

#endif // Include Guard: COLLISION_QUAD_TREE_H
