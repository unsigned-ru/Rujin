#include "RujinPCH.h"
#include "BoxColliderComponent.h"

#include "BoxCollider.h"
#include "CollisionQuadTree.h"
#include "GameObject.h"
#include "RenderService.h"
#include "Scene.h"
#include "ServiceLocator.h"

rujin::BoxColliderComponent::BoxColliderComponent(const glm::vec2& size, bool isStatic, const glm::vec2& pivot, CollisionLayer collisionLayer)
	: ColliderComponent()
	, m_pCollider(new BoxCollider(this, size, isStatic, pivot, collisionLayer))
{
}

rujin::BoxColliderComponent::~BoxColliderComponent()
{
	m_pGameObject->GetScene()->GetCollisionQuadTree()->Remove(m_pCollider);
	delete m_pCollider;
};

rujin::BoxCollider* rujin::BoxColliderComponent::GetCollider() const
{
	return m_pCollider;
}

#ifdef _DEBUG
void rujin::BoxColliderComponent::DrawDebug(const RenderService& renderer) const
{
	renderer.DrawRect(m_pCollider->GetRect(), 2.f);
}
#endif
