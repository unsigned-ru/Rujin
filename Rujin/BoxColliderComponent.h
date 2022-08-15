#ifndef BOX_COLLIDER_COMPONENT_H
#define BOX_COLLIDER_COMPONENT_H
#include "ColliderComponent.h"
#include "BoxCollider.h"

namespace rujin
{
	class RenderService;

	class BoxColliderComponent final : public ColliderComponent
	{
	public:
		explicit BoxColliderComponent(const glm::vec2& size, bool isStatic = true, const glm::vec2& pivot = { 0.5f, 0.5f }, CollisionLayer collisionLayer = CollisionLayer::BlockAll);
		~BoxColliderComponent() override;

		BoxColliderComponent(const BoxColliderComponent&) = delete;
		BoxColliderComponent(BoxColliderComponent&&) noexcept = delete;
		BoxColliderComponent& operator=(const BoxColliderComponent&) = delete;
		BoxColliderComponent& operator=(BoxColliderComponent&&) noexcept = delete;

		//Diferent return type, but still valid (because it's covariant)
		BoxCollider* GetCollider() const override;

	#ifdef _DEBUG
		void DrawDebug(const RenderService& renderer) const override;
	#endif

	private:
		BoxCollider* m_pCollider;
	};

}

#endif // Include Guard: BOX_COLLIDER_COMPONENT_H
