#pragma once
#include "Component.h"

namespace rujin
{
	class TransformComponent : public Component
	{
	public:
		explicit TransformComponent() = default;

#pragma region Global
		glm::vec3 GetPosition() const;
		void SetPosition(const glm::vec2& posXY);
		void SetPosition(const glm::vec3& posXYZ);

		glm::quat GetRotation() const;
		void SetRotation(const glm::quat& rot);

		glm::vec3 GetScale() const;
		void SetScale(const glm::vec3& scale);
#pragma endregion

#pragma region Local
		glm::vec3 GetLocalPosition() const;
		glm::vec3& GetLocalPosition();
		void SetLocalPosition(const glm::vec2& posXY);
		void SetLocalPosition(const glm::vec3& posXYZ);

		glm::quat GetLocalRotation() const;
		glm::quat& GetLocalRotation();
		void SetLocalRotation(const glm::quat& rot);

		glm::vec3 GetLocalScale() const;
		glm::vec3& GetLocalScale();
		void SetLocalScale(const glm::vec3& scale);
#pragma endregion
	protected:
		glm::vec3 m_localPosition{0.f ,0.f ,0.f};
		glm::quat m_localRotation{};
		glm::vec3 m_localScale{1.f, 1.f, 1.f};
	};

}
