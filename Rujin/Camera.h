#ifndef CAMERA_H
#define CAMERA_H


#include "TransformComponent.h"

namespace rujin
{
	class Camera final
	{
	public:
		explicit Camera(const glm::ivec2& size, const Transform& transform = {});

		void Project() const;

		void SetPosition(const Position& pos);
		void SetRotation(Rotation rot);

		const Position& GetPosition() const;
		Rotation GetRotation() const;
		const glm::vec2& GetSize() const;

	private:
		glm::vec2 m_Size;
		rujin::Transform m_Transform;
	};

}


#endif // Include Guard: CAMERA_H
