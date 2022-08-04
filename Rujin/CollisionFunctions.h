#ifndef COLLISION_FUNCTIONS_H
#define COLLISION_FUNCTIONS_H

#include "Structs.h"

namespace rujin::collision
{
	bool IsInside(const Rectf& rect, const Rectf& insideOf);
	bool IsOverlapping(const Rectf& a, const Rectf& b);

	/**
	 * \brief Checks if a line is intersecting a rectangle
	 * \param p1 Start point of line
	 * \param p2 End point of line
	 * \param r Axis aligned rectangle
	 * \param pNearT [OUT] Percentile on direction vector of closest intersection. (eg. \code InterectPointNear = p1 + (p2 - p1) * pNearT \endcode)
	 * \param pFarT [OUT] Percentile on direction vector of furthest intersection. (eg. \code InterectPointFar = p1 + (p2 - p1) * pFarT \endcode)
	 * \return 
	 */
	bool IsIntersecting(const glm::vec2& p1, const glm::vec2& p2, const Rectf& r, float* pNearT = nullptr, float* pFarT = nullptr);
}


#endif // Include Guard: COLLISION_FUNCTIONS_H
