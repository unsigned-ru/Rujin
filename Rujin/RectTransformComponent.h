#pragma once
#include "RujinPCH.h"
#include "TransformComponent.h"

namespace rujin
{
	class RectTransformComponent final : public TransformComponent
	{
	public:
		explicit RectTransformComponent(const std::weak_ptr<GameObject> gameObject);

		glm::vec2 GetSize() const;
		void SetSize(const glm::vec2& size);

		glm::vec2 GetPivot() const;
		void SetPivot(const glm::vec2& size);

	private:
		glm::vec2 m_Size{};
		/**
		 * vec2 ranging from 0.f to 1.f
		 */
		glm::vec2 m_Pivot{};
	};
}

