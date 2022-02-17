#pragma once
#include "Component.h"

namespace rujin
{
	class TransformComponent : public Component
	{
	public:
		explicit TransformComponent(const std::weak_ptr<GameObject> gameObject);

		const glm::vec3& GetPosition() const;
		glm::vec3& GetPosition();
		void SetPosition(const glm::vec3& pos);
		void SetPosition(const glm::vec2& posXY);

		const glm::quat& GetRotation() const;
		glm::quat& GetRotation();
		void SetRotation(const glm::quat& rot);

		const glm::vec3& GetScale() const;
		glm::vec3& GetScale();
		void SetScale(const glm::vec3& scale);

	protected:
		glm::vec3 m_Position{};
		glm::quat m_Rotation{};
		glm::vec3 m_Scale{};
	};
}
