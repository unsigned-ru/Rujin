#ifndef COLLIDER_H
#define COLLIDER_H
#include "CollisionLayers.h"
#include "CollisionQuadTree.h"
#include "Structs.h"


namespace rujin
{
	class ColliderComponent;

	class Collider
	{
	public:
		explicit Collider(ColliderComponent* pComponent, bool isStatic, const glm::vec2& pivot, CollisionLayer collisionLayer);
		virtual ~Collider() = default;

		Collider(const Collider&) = delete;
		Collider(Collider&&) noexcept = delete;
		Collider& operator=(const Collider&) = delete;
		Collider& operator=(Collider&&) noexcept = delete;

		virtual CollisionResult IsOverlapping(const Collider* pOther) const = 0;
		virtual void ResolveOverlap(const CollisionResult& result) const = 0;

		const glm::vec2& GetPivot() const;
		void SetPivot(const glm::vec2& pivot);

		bool IsStatic() const;

		ColliderComponent* GetComponent() const;
		CollisionLayer GetCollisionLayer() const;

	protected:
		CollisionLayer m_CollisionLayer;
		ColliderComponent* m_pComponent;
		glm::vec2 m_Pivot;
		bool m_IsStatic;

	private:
		friend class CollisionQuadTree;
		CollisionQuadTree* m_pOwningQuadTree = nullptr;
	};

}
#endif // Include Guard: COLLIDER_H
