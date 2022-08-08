#include "RujinPCH.h"
#include "BoxColliderComponent.h"

#include "BoxCollider.h"
#include "CollisionQuadTree.h"
#include "GameObject.h"
#include "RenderService.h"
#include "Scene.h"
#include "ServiceLocator.h"

rujin::BoxColliderComponent::BoxColliderComponent(const glm::vec2& size, const bool isStatic, const bool autoResolve, const glm::vec2& pivot)
	: ColliderComponent(autoResolve)
	, m_pCollider(new BoxCollider(this, size, isStatic, pivot))
{
}

rujin::BoxColliderComponent::~BoxColliderComponent()
{
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
