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

rujin::BoxColliderComponent::~BoxColliderComponent() = default;

rujin::Collider* rujin::BoxColliderComponent::GetCollider() const
{
	return m_pCollider;
}

void rujin::BoxColliderComponent::DrawDebug() const
{
	RenderService& rs = ServiceLocator::GetService<RenderService>();
	rs.SetColor({ 1.f, 1.f, 0.f, 1.f });
	rs.DrawRect(m_pCollider->GetRect(), 4.f);
	rs.SetColor({1.f, 1.f, 1.f, 1.f});
}

