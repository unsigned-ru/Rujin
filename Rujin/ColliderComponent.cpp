#include "RujinPCH.h"
#include "ColliderComponent.h"
#include "Collider.h"
#include "CollisionQuadTree.h"
#include "GameObject.h"
#include "Scene.h"

rujin::ColliderComponent::ColliderComponent(bool autoResolve)
	: m_AutoResolve(autoResolve)
{
}

void rujin::ColliderComponent::Start()
{
	m_pGameObject->GetScene()->GetCollisionQuadTree()->Insert(GetCollider());
}

void rujin::ColliderComponent::Draw() const
{
	if (m_DrawDebug)
		DrawDebug();
}

void rujin::ColliderComponent::OnOverlap(const CollisionResult& result)
{
	if (m_AutoResolve)
		GetCollider()->ResolveOverlap(result);
}

void rujin::ColliderComponent::EnableDebugDrawing(const bool enable)
{
	m_DrawDebug = enable;
}
