#ifndef TRANSFORM_COMPONENT_H
#define TRANSFORM_COMPONENT_H
#include "Component.h"
#include "Structs.h"
#include "Subject.h"

namespace rujin
{
	class TransformComponent final : public Component, public event::Subject
	{
	public:
		explicit TransformComponent();

#pragma region Global
		const Position& GetPosition() const;
		void SetPosition(const Position& pos);

		Rotation GetRotation() const;
		void SetRotation(Rotation rot);

		const Scale& GetScale() const;
		void SetScale(const Scale& scale);

		const Transform& GetTransform() const;
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

		const Transform& GetLocalTransform() const;
#pragma endregion

	private:
		void Start() override;
		void Update() override;
		void UpdateSelfAndChildren();

		TransformChanged m_TransformChanged{};
		Transform m_LocalTransform{};
		Transform m_GlobalTransform{};
	};

}
#endif
