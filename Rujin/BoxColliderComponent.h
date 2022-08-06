#ifndef BOX_COLLIDER_COMPONENT_H
#define BOX_COLLIDER_COMPONENT_H
#include "ColliderComponent.h"

namespace rujin
{
	class RenderService;
	class BoxCollider;

	class BoxColliderComponent final : public ColliderComponent
	{
	public:
		explicit BoxColliderComponent(const glm::vec2& size, bool isStatic = true, bool autoResolve = true, const glm::vec2& pivot = { 0.5f, 0.5f });
		~BoxColliderComponent() override;

		BoxColliderComponent(const BoxColliderComponent&) = delete;
		BoxColliderComponent(BoxColliderComponent&&) noexcept = delete;
		BoxColliderComponent& operator=(const BoxColliderComponent&) = delete;
		BoxColliderComponent& operator=(BoxColliderComponent&&) noexcept = delete;

		Collider* GetCollider() const override;

#ifdef _DEBUG
		void DrawDebug(const RenderService& renderer) const override;
#endif

	private:
		BoxCollider* m_pCollider;
	};

}

#endif // Include Guard: BOX_COLLIDER_COMPONENT_H
