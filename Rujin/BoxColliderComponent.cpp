#include "RujinPCH.h"
#include "BoxColliderComponent.h"

#include "BoxCollider.h"
#include "CollisionQuadTree.h"
#include "GameObject.h"
#include "Scene.h"

rujin::BoxColliderComponent::BoxColliderComponent(const glm::vec2& size, const bool isStatic, const bool autoResolve, const glm::vec2& pivot)
	: ColliderComponent(autoResolve)
	, m_pCollider(new BoxCollider(this, size, isStatic, pivot))
{
}

rujin::BoxColliderComponent::~BoxColliderComponent() = default;

rujin::Collider* rujin::BoxColliderComponent::GetCollider() const
{
	return m_pCollider;
}

void rujin::BoxColliderComponent::HandleCollision()
{
	//don't have to check for collisions if we are static.
	if (m_pCollider->IsStatic())
		return;

	//get colliders nearby by using quadtree.
	const std::vector<const Collider*> nearColliders = GameObject()->GetScene()->GetCollisionQuadTree()->Search(m_pCollider->GetRect());

	//check for overlaps
	for (const Collider* pCollider : nearColliders)
	{
		//avoid checking collisions with self.
		if (pCollider->GetComponent() == this)
			continue;
		
		const CollisionResult result = m_pCollider->IsOverlapping(pCollider);

		if (result.isColliding)
		{
			if (m_AutoResolve)
				m_pCollider->ResolveOverlap(result);

			//broadcast overlap to all components.
			GameObject()->BroadcastOverlap(result); 
		}
	}
}
