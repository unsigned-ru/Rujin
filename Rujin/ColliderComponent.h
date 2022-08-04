#ifndef COLLIDER_COMPONENT_H
#define COLLIDER_COMPONENT_H

#include "Component.h"

namespace rujin
{
	class Collider;

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
		void EnableDebugDrawing(bool enable = true);

		virtual Collider* GetCollider() const = 0;

	protected:
		bool m_AutoResolve;

	private:
		virtual void DrawDebug() const = 0;

		bool m_DrawDebug = false;
	};

}


#endif // Include Guard: COLLIDER_COMPONENT_H
