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

