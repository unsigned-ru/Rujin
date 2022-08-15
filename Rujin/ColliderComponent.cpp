#include "RujinPCH.h"
#include "ColliderComponent.h"
#include "Collider.h"
#include "CollisionQuadTree.h"
#include "GameObject.h"
#include "RenderService.h"
#include "Scene.h"
#include "ServiceLocator.h"

void rujin::ColliderComponent::Start()
{
	m_pGameObject->GetScene()->GetCollisionQuadTree()->Insert(GetCollider());
}

void rujin::ColliderComponent::Draw() const
{
#ifdef _DEBUG
	if (m_DrawDebug)
	{
		const RenderService& renderer = ServiceLocator::GetService<RenderService>();
		renderer.SetColor(m_DebugDrawingColor);
		DrawDebug(renderer);
		renderer.SetColor();
	}
#endif
}

#ifdef _DEBUG
void rujin::ColliderComponent::EnableDebugDrawing(const bool enable)
{
	m_DrawDebug = enable;
}

void rujin::ColliderComponent::SetDebugDrawingColor(const glm::vec4& color)
{
	m_DebugDrawingColor = color;
}
#endif