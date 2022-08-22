#include "RujinPCH.h"
#include "ColliderComponent.h"
#include "CollisionQuadTree.h"
#include "EngineEvents.h"
#include "GameObject.h"
#include "RenderService.h"
#include "Scene.h"
#include "ServiceLocator.h"

void rujin::ColliderComponent::Start()
{
	m_pGameObject->GetScene()->GetCollisionQuadTree()->Insert(GetCollider());
	m_pGameObject->AddObserver(this);
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

void rujin::ColliderComponent::OnNotify(const uint32_t identifier, const event::Data* pEventData)
{
	if (identifier == static_cast<uint32_t>(event::Identifier::OnGameObjectMoved))
	{
		const auto* pGameObjectMovedEvent = static_cast<const event::OnGameObjectMoved_t*>(pEventData);

		//remove collider from old scene
		m_pGameObject->GetScene()->GetCollisionQuadTree()->Remove(GetCollider());

		//add collider to new scene
		pGameObjectMovedEvent->pNewScene->GetCollisionQuadTree()->Insert(GetCollider());
	}

	if (identifier == static_cast<uint32_t>(event::Identifier::OnGameObjectDestroyed))
	{
		const auto* pGameObjectDestroyedEvent = static_cast<const event::OnGameObjectDestroyed_t*>(pEventData);

		//remove collider from scene
		if (pGameObjectDestroyedEvent->pObject == GameObject())
			m_pGameObject->GetScene()->GetCollisionQuadTree()->Remove(GetCollider());
	}
}