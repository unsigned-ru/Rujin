#pragma once
#include "TransformComponent.h"

namespace rujin
{
	class RectTransformComponent final : public TransformComponent
	{
	public:
		explicit RectTransformComponent(const std::weak_ptr<GameObject> gameObject);

		const glm::vec2& GetSize() const;
		glm::vec2& GetSize();
		void SetSize(const glm::vec2& size);

		const glm::vec2& GetPivot() const;
		glm::vec2& GetPivot();
		void SetPivot(const glm::vec2& pivot);

	private:
		glm::vec2 m_Size{};
		/**
		 * vec2 ranging from 0.f to 1.f
		 */
		glm::vec2 m_Pivot{};
	};
}

