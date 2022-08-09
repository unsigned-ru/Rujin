#ifndef MATH_H
#define MATH_H

#include <glm/gtx/common.hpp>
#include "Structs.h"

namespace rutils
{
	inline float GetAngleBetween(const glm::vec2& a, const glm::vec2& b)
	{
		return glm::acos(dot(a, b) / (length(a) * length(b)));
	}

	//Rotates given 2d vector by specified angle
	inline glm::vec2 Rotate(const glm::vec2& a, const float angle)
	{
		return glm::vec2
		{
			a.x * cos(angle) - a.y * sin(angle),
			a.x * sin(angle) + a.y * cos(angle)
		};
	}

	//Rotates given 2d vector by specified angle
	inline rujin::Rectf Rotate(const rujin::Rectf& r, const float angle, const glm::vec2& pivot)
	{
		if (abs(angle) < FLT_EPSILON) 
			return r;

		const auto oldVerts = r.GetVertices();

		//calculate pivot position.
		const glm::vec2 pivotPos{ r.left + pivot.x * r.width, r.bottom + pivot.y * r.height };

		//calculate new rotated vertices.
		rujin::Rectf::Vertices newVerts{};
		newVerts.bottomLeft = pivotPos + Rotate(oldVerts.bottomLeft - pivotPos, angle);
		newVerts.bottomRight = pivotPos + Rotate(oldVerts.bottomRight - pivotPos, angle);
		newVerts.topRight = pivotPos + Rotate(oldVerts.topRight - pivotPos, angle);
		newVerts.topLeft = pivotPos + Rotate(oldVerts.topLeft - pivotPos, angle);

		// return new rect
		return rujin::Rectf(newVerts.bottomLeft.x, newVerts.bottomLeft.y, newVerts.bottomRight.x - newVerts.bottomLeft.x, newVerts.topLeft.y - newVerts.bottomLeft.y);
	}

	inline void Decompose(const glm::mat3& matrix, rujin::Position& translation, rujin::Rotation& rotation, rujin::Scale& scale)
	{
		translation.x = matrix[2].x;
		translation.y = matrix[2].y;

		scale.x = length(matrix[0]);
		scale.y = length(matrix[1]);

		rotation = atan2f(matrix[0][1],matrix[0][0]);
	}

}

#endif // Include Guard: MATH_H
