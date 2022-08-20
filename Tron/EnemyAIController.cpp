#include "TronPCH.h"
#include "EnemyAIController.h"

#include "Collider.h"
#include "ColliderComponent.h"
#include "CollisionLayers.h"
#include "CollisionQuadTree.h"
#include "GameObject.h"
#include "Scene.h"

void EnemyAIController::Start()
{
	Component::Start();

	GameObject()->AddTag("Enemy");
}

bool EnemyAIController::IsInRange(rujin::GameObject* pObj, const float range) const
{
	return distance(GameObject()->GetTransform().GetPosition(), pObj->GetTransform().GetPosition()) < range;
}

bool EnemyAIController::IsInLineOfSight(rujin::GameObject* pObj, const std::vector<const Collider*>& ignore) const
{
	const glm::vec2 rayStart = GameObject()->GetTransform().GetPosition();
	const glm::vec2 rayEnd = pObj->GetTransform().GetPosition();

	const Collider* pHitCollider;
	if
	(
		GameObject()->GetScene()->GetCollisionQuadTree()->RaycastByLayer
		(
			rayStart,
			rayEnd,
			CollisionLayer::Default,
			ignore,
			nullptr,
			&pHitCollider
		)
	)
	{
		//we hit something, check if it's the object we wanted to hit...
		return pHitCollider->GetComponent()->GameObject() == pObj;
	}

	return true;
}

Direction EnemyAIController::GetDirectionToObject(rujin::GameObject* pObj) const
{
	const glm::vec2 currPos = GameObject()->GetTransform().GetPosition();
	const glm::vec2 targetPos = pObj->GetTransform().GetPosition();

	return VectorToDirection(targetPos - currPos);
}
