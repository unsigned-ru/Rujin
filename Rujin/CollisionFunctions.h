#ifndef COLLISION_FUNCTIONS_H
#define COLLISION_FUNCTIONS_H

#include "Structs.h"

namespace rujin::collision
{
	bool IsInside(const Rectf& rect, const Rectf& insideOf);
	bool IsOverlapping(const Rectf& a, const Rectf& b);
}


#endif // Include Guard: COLLISION_FUNCTIONS_H
