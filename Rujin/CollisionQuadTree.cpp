#include "RujinPCH.h"
#include "CollisionQuadTree.h"

#include "BoxCollider.h"
#include "ColliderComponent.h"
#include "CollisionFunctions.h"
#include "Component.h"
#include "EventData.h"
#include "GameObject.h"
#include "RenderService.h"
#include "ServiceLocator.h"
#include "Rutils/General.h"

rujin::CollisionQuadTree::CollisionQuadTree(const Rectf& bounds) : CollisionQuadTree(40, 5, 0, bounds, nullptr) {}

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
	if (!pCollider)
		LOG_ERROR("Error inserting, pCollider was nullptr.");

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

	//at this point we know the pCollider should be inserted in the current node
	m_Colliders.emplace_back(pCollider); //insert it

	
	if (!pCollider->IsStatic()) //if it dynamic
		//observe transform of collider to ensure we update our quadtree if we update the position of the collider.
		pCollider->GetComponent()->GameObject()->GetTransform()->AddObserver(this);
	

	//since we inserted a pCollider, we need to check if this new insertion exceeds our node limits.
	if (m_Colliders.size() > m_MaxObjects && m_Level < m_MaxLevels && m_Children[0] == nullptr)
	{
		//if it does,
		Split(); // we need to split

		//because we have split, we need to recalculate what new node each pCollider
		//now belongs to.
		auto it = m_Colliders.begin();
		while (it != m_Colliders.end())
		{
			Collider* pCurrObj = *it;

			if (const BoxCollider* boxObject = dynamic_cast<BoxCollider*>(pCurrObj); boxObject)
			{
				const int8_t indexToPlaceObject = GetChildIndexForObject(boxObject->GetRect());


				if (indexToPlaceObject != s_ThisTree)
				{
					//we need to move it inside one of the child trees.
					m_Children[indexToPlaceObject]->Insert(pCurrObj); //insert it.

					it = m_Colliders.erase(it); // remove it from this tree's vector

					if (!pCollider->IsStatic()) //if it dynamic
						pCollider->GetComponent()->GameObject()->GetTransform()->RemoveObserver(this); //remove this tree as observer.

					continue; //to prevent increment
				}
			}
			else
				LOG_WARNING("Unimplemented Collider detected. Did you forget to implement?");

			++it;
		}
	}

	return pCollider;
}

void rujin::CollisionQuadTree::Remove(Collider* pCollider)
{
	if (const BoxCollider* pBoxCollider = dynamic_cast<BoxCollider*>(pCollider); pBoxCollider)
	{
		const int8_t idx = GetChildIndexForObject(pBoxCollider->GetRect());

		//check if collider belongs to this node and has no children...
		if (idx == s_ThisTree || m_Children[idx] == nullptr)
		{
			//find the collider by checking if they have the same "owning" component.
			Component* pColliderComp = pCollider->GetComponent();
			const auto it = std::ranges::find_if(m_Colliders, [pColliderComp](const Collider* pCurrCollider) { return pCurrCollider->GetComponent() == pColliderComp; });

			if (it != m_Colliders.end()) //if we found the collider in this tree's vector...
			{
				m_Colliders.erase(it); //...remove it
				
				if (!pCollider->IsStatic()) //if it dynamic
					//remove this tree as observer of the transform
					pCollider->GetComponent()->GameObject()->GetTransform()->RemoveObserver(this);
			}
		}
		else
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

	std::vector<const Collider*>actualOverlaps{};

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
	overlappingColliders.insert(overlappingColliders.end(), m_Colliders.begin(), m_Colliders.end());

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
}

void rujin::CollisionQuadTree::OnNotify(const uint32_t identifier, const event::Data* pEventData)
{
	if (identifier == static_cast<uint32_t>(event::Identifier::OnTransformChanged))
	{
		auto* onTransformChanged = static_cast<const event::OnTransformChanged_t*>(pEventData);

		//make sure the collider remains in the correct quadtree.

		//Get the collider from the collider component.
		if (const ColliderComponent* pColliderComp = onTransformChanged->pTransformComponent->GameObject()->GetComponent<ColliderComponent>(); pColliderComp)
		{
			//found the collider comp.
			Collider* pCollider = pColliderComp->GetCollider();

			//remove it from this quad-tree.
			Remove(pCollider);

			//find root (parent) tree.
			CollisionQuadTree* pRoot = this;

			while (pRoot->m_pParent)
				pRoot = pRoot->m_pParent;

			//insert it again in the root (parent) tree!
			pRoot->Insert(pCollider);
		}
	}
}
