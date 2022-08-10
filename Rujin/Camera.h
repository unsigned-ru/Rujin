#ifndef CAMERA_H
#define CAMERA_H


#include "Transform.h"

namespace rujin
{
	class Camera final
	{
	public:
		explicit Camera(const glm::ivec2& size);

		void Project() const;

		Transform& GetTransform();
		const glm::vec2& GetSize() const;

	private:
		glm::vec2 m_Size;
		Transform m_Transform;
	};

}


#endif // Include Guard: CAMERA_H
