#ifndef COLLIDER_H
#define COLLIDER_H
#include "Structs.h"


namespace rujin
{
	class Component;

	class Collider
	{
	public:
		explicit Collider(Component* pComponent, bool isStatic, const glm::vec2& pivot);
		virtual ~Collider() = default;

		Collider(const Collider&) = delete;
		Collider(Collider&&) noexcept = delete;
		Collider& operator=(const Collider&) = delete;
		Collider& operator=(Collider&&) noexcept = delete;

		virtual CollisionResult IsOverlapping(const Collider* pOther) = 0;
		virtual void ResolveOverlap(const CollisionResult& result) = 0;

		const glm::vec2& GetPivot() const;
		void SetPivot(const glm::vec2& pivot);

		bool IsStatic() const;

		Component* GetComponent() const;

	protected:
		Component* m_pComponent;
		glm::vec2 m_Pivot;
		bool m_IsStatic;
	};

}
#endif // Include Guard: COLLIDER_H
