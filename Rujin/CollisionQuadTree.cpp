#include "RujinPCH.h"
#include "CollisionQuadTree.h"

#include "BoxCollider.h"
#include "ColliderComponent.h"
#include "CollisionFunctions.h"
#include "Component.h"
#include "EngineEvents.h"
#include "GameObject.h"
#include "RenderService.h"
#include "ServiceLocator.h"

rujin::CollisionQuadTree::CollisionQuadTree(const Rectf& bounds) : CollisionQuadTree(15, 5, 0, bounds, nullptr) {}

rujin::CollisionQuadTree::CollisionQuadTree
(
	const uint32_t maxObjects, const uint32_t maxLevels, const uint32_t level,
	const Rectf& bounds, CollisionQuadTree* pParent
)
	: m_MaxObjects(maxObjects)
	, m_MaxLevels(maxLevels)
	, m_pParent(pParent)
	, m_Level(level)
	, m_Bounds(bounds)
{
}

rujin::Collider* rujin::CollisionQuadTree::Insert(Collider* pCollider)
{
	ASSERT_RV(pCollider, nullptr);

	if (!m_pParent && !pCollider->IsStatic()) //if we are root and collider isn't static.
		//observe transform of collider to ensure we update our quadtree if we update the position of the collider.
		pCollider->GetComponent()->GameObject()->GetTransform().AddObserver(this); //NOTE: only root should observe.

	//check if we have any children nodes. if we have 1 child node,
	//we assume we have 4, since they only ever get created in pairs of 4.
	if (m_Children[0])
	{
		if (const BoxCollider* boxObject = dynamic_cast<BoxCollider*>(pCollider); boxObject)
		{
			//Find index of node pCollider should belong to.
			const int8_t indexToPlaceObject = GetChildIndexForObject(boxObject->GetRect()); // 2

			if (indexToPlaceObject != s_ThisTree) //if index isn't -1 (this tree)
				return m_Children[indexToPlaceObject]->Insert(pCollider); //insert in child tree
		}
	}

	//collider should be inserted into this node....
	//if inserting is going to make us exceed the capacity...
	if (m_Colliders.GetVector().size() + 1 > m_MaxObjects && m_Level < m_MaxLevels && m_Children[0] == nullptr)
	{
		Split(); // we need to split
		Insert(pCollider); // call insert again, this time there will be enough space!
	}
	else
	{
		//we haven't reached our capacity, insert it into this node.
		m_Colliders.GetVector().emplace_back(pCollider);
		pCollider->m_pOwningQuadTree = this;
	}

	return pCollider;
}

void rujin::CollisionQuadTree::Remove(Collider* pCollider)
{
	if (!m_pParent && !pCollider->IsStatic()) //if we are root and collider isn't static.
		//stop observing the colliders transform, we no longer need to update it's position in the quadtree
		pCollider->GetComponent()->GameObject()->GetTransform().RemoveObserver(this); //NOTE: only root should observe.

	if (const BoxCollider* pBoxCollider = dynamic_cast<BoxCollider*>(pCollider); pBoxCollider)
	{
		const int8_t idx = GetChildIndexForObject(pBoxCollider->GetRect());

		//check if collider belongs to this node and has no children...
		if (idx == s_ThisTree || m_Children[idx] == nullptr)
		{
			//Collider must be in this node!
			//Find it by comparing the owning component.

			Component* pColliderComp = pCollider->GetComponent();

			const auto it = std::ranges::find_if
			(m_Colliders.GetVector(), 
				[pColliderComp](const Collider* pCurrCollider)
				{
					return pCurrCollider->GetComponent() == pColliderComp;
				}
			);

			if (it != m_Colliders.GetVector().end()) //if we found the collider in this tree's vector...
			{
				m_Colliders.GetVector().erase(it); //...remove it
			}
 		}
		else //search it in our children
			return m_Children[idx]->Remove(pCollider);
	}
	else
		LOG_WARNING("Unimplemented Collider detected. Did you forget to implement?");
}

std::vector<const rujin::Collider*> rujin::CollisionQuadTree::Search(const Rectf& area)
{
	//get all colliders inside of nodes overlapping the search area
	std::vector<const Collider*> possibleOverlaps{};
	Search(area, possibleOverlaps);

	std::vector<const Collider*> actualOverlaps{};

	//check in detail if the possible overlaps, actually overlap.
	for (const Collider* pCollider : possibleOverlaps)
	{
		if (auto* pBoxCollider = dynamic_cast<const BoxCollider*>(pCollider); pCollider)
		{
			if (collision::IsOverlapping(pBoxCollider->GetRect(), area))
				actualOverlaps.emplace_back(pCollider);
		}
		else
			LOG_WARNING("Unimplemented Collider detected. Did you forget to implement?");
			
	}

	return actualOverlaps;
}

const rujin::Rectf& rujin::CollisionQuadTree::GetBounds() const
{
	return m_Bounds;
}

void rujin::CollisionQuadTree::HandleCollision(CollisionQuadTree* pRoot)
{
	for (Collider* pCollider : m_Colliders.GetVector())
	{
		if (pCollider->IsStatic())
			continue;

		if (BoxCollider* pBoxCollider = dynamic_cast<BoxCollider*>(pCollider); pBoxCollider)
		{
			Rectf colliderBounds = pBoxCollider->GetRect();

			const std::vector<const Collider*> possibleOverlaps = pRoot->Search(colliderBounds);

			//check for overlaps
			for (const Collider* pPossibleOverlapCollider : possibleOverlaps)
			{
				//avoid checking collisions with self.
				if (pCollider == pPossibleOverlapCollider)
					continue;

				CollisionResult result = pBoxCollider->IsOverlapping(pPossibleOverlapCollider);

				if (result.isColliding)
				{
					switch (GetResponseForCollisionLayer(pBoxCollider->GetCollisionLayer(), result.other->GetCollisionLayer()))
					{
					case CollisionResponse::Block: //intentional break-through
						pBoxCollider->ResolveOverlap(result);
						pBoxCollider->GetComponent()->GameObject()->GetTransform().UpdateSelfAndChildren();
					case CollisionResponse::Overlap:
						pBoxCollider->GetComponent()->GameObject()->BroadcastOverlap(result);
						break;
					case CollisionResponse::Ignore: //intentional break-through
					default: break;
					}


					if (GetResponseForCollisionLayer(result.other->GetCollisionLayer(), pBoxCollider->GetCollisionLayer()) != CollisionResponse::Ignore)
					{
						//also let the other know we overlapped!
						result.other = pBoxCollider;

						//invert collision direction
						result.collisionDirection = GetInverted(result.collisionDirection);

						pPossibleOverlapCollider->GetComponent()->GameObject()->BroadcastOverlap(result);
					}
				}
			}
		}
		else
			LOG_WARNING("Unimplemented Collider detected. Did you forget to implement?");
	}

	//if we have children
	if (m_Children[0])
		//recursion! handle children tree collision.
		for (uint8_t i = 0u; i < 4u; ++i)
			m_Children[i]->HandleCollision(pRoot);
}

bool rujin::CollisionQuadTree::Raycast(const glm::vec2& p1, const glm::vec2& p2, const std::vector<const Collider*>& ignore, glm::vec2* pIntesection, const Collider** ppHitCollider)
{
	std::vector<const Collider*> possibleIntersections{};

	//get all colliders inside of intersecting cells.
	Search(p1, p2, possibleIntersections);

	//sort them by distance to the starting point.
	std::ranges::sort
	(
		possibleIntersections, 
		[&p1](const Collider* c1, const Collider* c2)
		{
			return distance2(p1, c1->GetComponent()->GameObject()->GetTransform().GetPosition()) < distance2(p1, c2->GetComponent()->GameObject()->GetTransform().GetPosition());
		}
	);

	
	//now that we have sorted them by distance, check for intersections and return on first interstection!
	for (const Collider* pPossibleCollider : possibleIntersections)
	{
		if (const BoxCollider* pPossibleBoxCollider = dynamic_cast<const BoxCollider*>(pPossibleCollider); pPossibleBoxCollider)
		{
			if (std::ranges::find(ignore, pPossibleCollider) != ignore.end())
				continue; //ignore the colliders in the ignore vector.

			const Rectf colliderRect = pPossibleBoxCollider->GetRect();
			float nearT{};

			// if both points are inside of collider...
			if (collision::IsInside(p1, colliderRect) && collision::IsInside(p2, colliderRect))
			{
				if (ppHitCollider)
					*ppHitCollider = pPossibleBoxCollider;

				return true;
			}

			//if we intersect
			if (collision::IsIntersecting(p1, p2, true, colliderRect, &nearT))
			{
				if (pIntesection)
					//calculate intersection point.
					(*pIntesection) = p1 + (p2 - p1) * nearT;

				if (ppHitCollider)
					*ppHitCollider = pPossibleBoxCollider;

				return true;
			}
		}
		else
			LOG_WARNING("Unimplemented Collider detected. Did you forget to implement?");
	}

	return false;
}

bool rujin::CollisionQuadTree::RaycastByLayer(const glm::vec2& p1, const glm::vec2& p2, CollisionLayer layer, const std::vector<const Collider*>& ignore, glm::vec2* pIntesection, const Collider** ppHitCollider)
{
	std::vector<const Collider*> possibleIntersections{};

	//get all colliders inside of intersecting cells.
	Search(p1, p2, possibleIntersections);

	//remove those that do not belong to the specified layer
	auto removeIt = std::remove_if
	(
		possibleIntersections.begin(),
		possibleIntersections.end(),
		[layer](const Collider* pCollider)
		{
			return pCollider->GetCollisionLayer() != layer;
		}
	);

	possibleIntersections.erase(removeIt, possibleIntersections.end());

	//sort them by distance to the starting point.
	std::ranges::sort
	(
		possibleIntersections,
		[&p1](const Collider* c1, const Collider* c2)
		{
			return distance2(p1, c1->GetComponent()->GameObject()->GetTransform().GetPosition()) < distance2(p1, c2->GetComponent()->GameObject()->GetTransform().GetPosition());
		}
	);


	//now that we have sorted them by distance, check for intersections and return on first interstection!
	for (const Collider* pPossibleCollider : possibleIntersections)
	{
		if (const BoxCollider* pPossibleBoxCollider = dynamic_cast<const BoxCollider*>(pPossibleCollider); pPossibleBoxCollider)
		{
			if (std::ranges::find(ignore, pPossibleCollider) != ignore.end())
				continue; //ignore the colliders in the ignore vector.

			const Rectf colliderRect = pPossibleBoxCollider->GetRect();
			float nearT{};

			// if both points are inside of collider...
			if (collision::IsInside(p1, colliderRect) && collision::IsInside(p2, colliderRect))
			{
				if (ppHitCollider)
					*ppHitCollider = pPossibleBoxCollider;

				return true;
			}

			//if we intersect
			if (collision::IsIntersecting(p1, p2, true, colliderRect, &nearT))
			{
				if (pIntesection)
					//calculate intersection point.
					(*pIntesection) = p1 + (p2 - p1) * nearT;

				if (ppHitCollider)
					*ppHitCollider = pPossibleBoxCollider;

				return true;
			}
		}
		else
			LOG_WARNING("Unimplemented Collider detected. Did you forget to implement?");
	}

	return false;
}


void rujin::CollisionQuadTree::DrawDebug() const
{
	if(m_Children[0])
	{
		RenderService& rs = ServiceLocator::GetService<RenderService>();

		//if we have children, draw their rects, and tell the children to also draw their rects.
		for (uint8_t i = 0; i < 4; ++i)
		{
			//set random debug color
			rs.SetColor({1.f, 0.f, 0.f, 1.f});
			rs.DrawRect(m_Children[i]->GetBounds());

			m_Children[i]->DrawDebug();
			rs.SetColor({ 1.f, 1.f, 1.f, 1.f });
		}
	}
}


void rujin::CollisionQuadTree::Search(const Rectf& area, std::vector<const Collider*>& overlappingColliders)
{
	//add all objects in this node to the vector.
	overlappingColliders.insert(overlappingColliders.end(), m_Colliders.GetVector().begin(), m_Colliders.GetVector().end());

	if (m_Children[0]) //if we have child nodes...
	{
		//will return -1 if search area does not fit completely in node
		//or if we don't have child nodes (which we know we have with the above check)
		int8_t idx = GetChildIndexForObject(area);

		if (idx == s_ThisTree) //if it doesn't entirely fit in a child...
		{
			for (size_t i = 0; i < 4; ++i) //search in all children...
			{
				//that overlap the search area
				if (collision::IsOverlapping(m_Children[i]->GetBounds(), area))
					m_Children[i]->Search(area, overlappingColliders);
			}
		}
		else
		{
			//search area did fully fit in a child node!

			//search that node!
			m_Children[idx]->Search(area, overlappingColliders);
		}
	}
}

void rujin::CollisionQuadTree::Search(const glm::vec2& p1, const glm::vec2& p2, std::vector<const Collider*>& overlappingColliders)
{
	//add all objects in this node to the vector.
	overlappingColliders.insert(overlappingColliders.end(), m_Colliders.GetVector().begin(), m_Colliders.GetVector().end());

	if (m_Children[0]) //if we have child nodes...
	{
		for (uint8_t i = 0; i < 4; ++i)
		{
			//if both points are inside...
			//or
			//the line intersects the...
			//...child bounding box.
			const Rectf& childBounds = m_Children[i]->GetBounds();
			if ((collision::IsInside(p1, childBounds) && collision::IsInside(p2, childBounds)) || collision::IsIntersecting(p1, p2, true, childBounds))
			{
				//search in the child.
				m_Children[i]->Search(p1, p2, overlappingColliders);
			}
		}
	}

}

int8_t rujin::CollisionQuadTree::GetChildIndexForObject(const Rectf& colliderBounds) const
{
	int8_t idx = -1;

	//if we have children, use their existing rects
	if (m_Children[0])
	{
		for (int8_t i = 0; i < 4; ++i)
		{
			if (collision::IsInside(colliderBounds, m_Children[i]->GetBounds()))
			{
				idx = i;
				break;
			}
		}
	}
	else
	{
		//we don't have existing children, calculate the 4 rects.
		Rectf rects[4];
		const glm::vec2 childSize{ m_Bounds.width / 2.f, m_Bounds.height / 2.f };

		rects[s_ChildIdxNE] = Rectf{ m_Bounds.left + childSize.x, m_Bounds.bottom + childSize.y, childSize.x, childSize.y };
		rects[s_ChildIdxSE] = Rectf{ m_Bounds.left + childSize.x, m_Bounds.bottom, childSize.x, childSize.y };
		rects[s_ChildIdxSW] = Rectf{ m_Bounds.left, m_Bounds.bottom, childSize.x, childSize.y };
		rects[s_ChildIdxNW] = Rectf{ m_Bounds.left, m_Bounds.bottom + childSize.y, childSize.x, childSize.y };

		//check if bounds inside of any of the child rects.
		for (int8_t i = 0; i < 4; ++i)
		{
			if (collision::IsInside(colliderBounds, rects[i]))
			{
				idx = i;
				break;
			}
		}
	}

	return idx;
}

void rujin::CollisionQuadTree::Split()
{
	//create children...
	const glm::vec2 childSize{ m_Bounds.width / 2.f, m_Bounds.height / 2.f };

	m_Children[s_ChildIdxNE] = std::make_unique<CollisionQuadTree>(
		m_MaxObjects, m_MaxLevels, m_Level + 1,
		Rectf{ m_Bounds.left + childSize.x, m_Bounds.bottom + childSize.y, childSize.x, childSize.y },
		this
	);

	m_Children[s_ChildIdxSE] = std::make_unique<CollisionQuadTree>(
		m_MaxObjects, m_MaxLevels, m_Level + 1,
		Rectf{ m_Bounds.left + childSize.x, m_Bounds.bottom, childSize.x, childSize.y },
		this
	);

	m_Children[s_ChildIdxSW] = std::make_unique<CollisionQuadTree>(
		m_MaxObjects, m_MaxLevels, m_Level + 1,
		Rectf{ m_Bounds.left, m_Bounds.bottom, childSize.x, childSize.y },
		this
	);

	m_Children[s_ChildIdxNW] = std::make_unique<CollisionQuadTree>(
		m_MaxObjects, m_MaxLevels, m_Level + 1,
		Rectf{ m_Bounds.left, m_Bounds.bottom + childSize.y, childSize.x, childSize.y },
		this
	);

	//because we have split, we need to recalculate what new node each pCollider
	//now belongs to.
	auto it = m_Colliders.GetVector().begin();
	while (it != m_Colliders.GetVector().end())
	{
		Collider* pCurrObj = *it;

		if (const BoxCollider* boxObject = dynamic_cast<BoxCollider*>(pCurrObj); boxObject)
		{
			const int8_t indexToPlaceObject = GetChildIndexForObject(boxObject->GetRect());

			if (indexToPlaceObject != s_ThisTree)
			{
				//we need to move it inside one of the child trees.
				m_Children[indexToPlaceObject]->Insert(pCurrObj); //insert it.

				it = m_Colliders.GetVector().erase(it); // remove it from this tree's vector

				if (!pCurrObj->IsStatic()) //if it dynamic
					pCurrObj->GetComponent()->GameObject()->GetTransform().RemoveObserver(this); //remove this tree as observer.

				continue; //to prevent increment
			}
		}
		else
			LOG_WARNING("Unimplemented Collider detected. Did you forget to implement?");

		++it;
	}
}

void rujin::CollisionQuadTree::OnNotify(const uint32_t identifier, const event::Data* pEventData)
{
	if (identifier == static_cast<uint32_t>(event::Identifier::OnTransformChanged))
	{
		auto* onTransformChanged = static_cast<const event::OnTransformChanged_t*>(pEventData);

		//make sure the collider remains in the correct quadtree.

		//Get the collider from the collider component.
		if (const ColliderComponent* pColliderComp = onTransformChanged->transform.GetGameObject()->GetComponent<ColliderComponent>(); pColliderComp)
		{
			//found the collider comp.
			Collider* pCollider = pColliderComp->GetCollider();

			//remove it from this quad-tree.
			Remove_(pCollider);

			//insert it again in the root (parent) tree!
			Insert(pCollider);
		}
	}
}

rujin::CollisionQuadTree* rujin::CollisionQuadTree::GetRoot()
{
	CollisionQuadTree* pRoot = this;

	while (pRoot->m_pParent)
		pRoot = pRoot->m_pParent;

	return pRoot;
}

void rujin::CollisionQuadTree::Remove_(Collider* pCollider)
{
	if (!m_pParent && !pCollider->IsStatic()) //if we are root and collider isn't static.
		//stop observing the colliders transform, we no longer need to update it's position in the quadtree
		pCollider->GetComponent()->GameObject()->GetTransform().RemoveObserver(this); //NOTE: only root should observe.

	auto& owningTree = pCollider->m_pOwningQuadTree->m_Colliders.GetVector();
	owningTree.erase(std::ranges::find(owningTree, pCollider));
}
