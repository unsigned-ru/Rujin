#ifndef BOX_COLLIDER_H
#define BOX_COLLIDER_H
#include "Collider.h"

namespace rujin
{
	class BoxCollider final : public Collider
	{
	public:
		explicit BoxCollider(Component* pComponent, const glm::vec2& size, bool isStatic = true, const glm::vec2& pivot = {0.5f, 0.5f});
		~BoxCollider() override = default;

		BoxCollider(const BoxCollider&) = delete;
		BoxCollider(BoxCollider&&) noexcept = delete;
		BoxCollider& operator=(const BoxCollider&) = delete;
		BoxCollider& operator=(BoxCollider&&) noexcept = delete;

		const glm::vec2& GetSize() const;
		void SetSize(const glm::vec2& size);

		Rectf GetRect() const;

		CollisionResult IsOverlapping(const Collider* pOther) override;
		void ResolveOverlap(const CollisionResult& result) override;

	private:
		glm::vec2 m_Size;
	};
}

#endif // Include Guard: BOX_COLLIDER_H


