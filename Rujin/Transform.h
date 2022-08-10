#ifndef TRANSFORM_COMPONENT_H
#define TRANSFORM_COMPONENT_H
#include "Structs.h"
#include "Subject.h"

namespace rujin
{
	class GameObject;

	class Transform final : public event::Subject
	{
	public:
		explicit Transform(GameObject* pOwner);

#pragma region Global
		const Position& GetPosition() const;
		void SetPosition(const Position& pos);

		Rotation GetRotation() const;
		void SetRotation(Rotation rot);

		const Scale& GetScale() const;
		void SetScale(const Scale& scale);
#pragma endregion

#pragma region Local
		const Position& GetLocalPosition() const;
		void SetLocalPosition(const Position& pos);
		void AddLocalPosition(const Position& pos);

		Rotation GetLocalRotation() const;
		void SetLocalRotation(Rotation rot);
		void AddLocalRotation(Rotation rot);

		const Scale& GetLocalScale() const;
		void SetLocalScale(const Scale& scale);
		void AddLocalScale(const Scale& scale);
#pragma endregion

		GameObject* GetGameObject() const;

		void UpdateSelfAndChildren();

	private:
		void ApplyTransformationsToSelfAndChildren();

		glm::mat3 GetLocalModelMatrix();

		Position m_LocalPosition{};
		Rotation m_LocalRotation{};
		Scale m_LocalScale{ 1.f, 1.f };

		Position m_GlobalPosition{};
		Rotation m_GlobalRotation{};
		Scale m_GlobalScale{ 1.f, 1.f };

		glm::mat3 m_WorldMatrix = glm::mat4(1.0f);

		TransformChanged m_TransformChanged{};

		GameObject* const m_pGameObject;
	};

}
#endif
