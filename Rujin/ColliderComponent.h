#ifndef COLLIDER_COMPONENT_H
#define COLLIDER_COMPONENT_H

#include "Component.h"

namespace rujin
{
	class RenderService;

	class ColliderComponent : public Component
	{
	public:
		explicit ColliderComponent(bool autoResolve = true);
		~ColliderComponent() override = default;

		ColliderComponent(const ColliderComponent&) = delete;
		ColliderComponent(ColliderComponent&&) noexcept = delete;
		ColliderComponent& operator=(const ColliderComponent&) = delete;
		ColliderComponent& operator=(ColliderComponent&&) noexcept = delete;

		void Start() override;
		void Draw() const override;
		void OnOverlap(const CollisionResult&) override;

#ifdef _DEBUG
		void EnableDebugDrawing(bool enable = true);
		void SetDebugDrawingColor(const glm::vec4& color);
#endif

		virtual Collider* GetCollider() const = 0;

	protected:
		bool m_AutoResolve;

	private:
#ifdef _DEBUG
		virtual void DrawDebug(const RenderService& renderer) const = 0;

		bool m_DrawDebug = false;
		glm::vec4 m_DebugDrawingColor = { 1.f, 1.f, 0.f, 1.f };
#endif
	};

}


#endif // Include Guard: COLLIDER_COMPONENT_H
