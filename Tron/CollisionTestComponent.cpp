#include "TronPCH.h"
#include "CollisionTestComponent.h"

#include "BoxCollider.h"
#include "BoxColliderComponent.h"
#include "GameObject.h"
#include "RenderService.h"
#include "ServiceLocator.h"

void CollisionTestComponent::Draw() const
{
	RenderService& rs = ServiceLocator::GetService<RenderService>();

	BoxColliderComponent* pBoxColliderComp = GameObject()->GetComponent<BoxColliderComponent>();

	rs.SetColor({ 0.f, 0.f, 1.f, 1.f });
	rs.FillRect(static_cast<BoxCollider*>(pBoxColliderComp->GetCollider())->GetRect());
	rs.SetColor({ 1.f, 1.f, 1.f, 1.f});
}

void CollisionTestComponent::OnOverlap(const CollisionResult&)
{
	LOG_DEBUG("Overlapping!");
}
