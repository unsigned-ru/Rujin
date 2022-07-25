#pragma once
#include "Component.h"
#include "Structs.h"

namespace rujin
{
	class TransformComponent final : public Component
	{
	public:
		explicit TransformComponent() = default;

#pragma region Global
		Position GetPosition() const;
		void SetPosition(const Position& pos);

		Rotation GetRotation() const;
		void SetRotation(Rotation rot);

		glm::vec2 GetScale() const;
		void SetScale(const Scale& scale);

		Transform GetTransform() const;
#pragma endregion

#pragma region Local
		const Position& GetLocalPosition() const;
		void SetLocalPosition(const Position& pos);

		Rotation GetLocalRotation() const;
		void SetLocalRotation(Rotation rot);

		const Scale& GetLocalScale() const;
		void SetLocalScale(const Scale& scale);

		const Transform& GetLocalTransform() const;
#pragma endregion

		Transform m_LocalTransform{};
	};

}
