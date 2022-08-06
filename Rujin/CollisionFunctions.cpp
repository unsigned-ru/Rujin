#include "RujinPCH.h"
#include "CollisionFunctions.h"

bool rujin::collision::IsInside(const glm::vec2& point, const Rectf& rect)
{
	const Rectf::Vertices rectVertices = rect.GetVertices();

	return point > rectVertices.bottomLeft && point < rectVertices.topRight;
}

bool rujin::collision::IsInside(const Rectf& rect, const Rectf& insideOf)
{
	const Rectf::Vertices rectVertices = rect.GetVertices();
	const Rectf::Vertices insideOfVertices = insideOf.GetVertices();

	//if rBL > ioBL && rTR < ioTR
	return
		rectVertices.bottomLeft.x > insideOfVertices.bottomLeft.x &&	
		rectVertices.bottomLeft.y > insideOfVertices.bottomLeft.y &&
		rectVertices.topRight.x < insideOfVertices.topRight.x &&
		rectVertices.topRight.y < insideOfVertices.topRight.y;
}

bool rujin::collision::IsOverlapping(const Rectf& a, const Rectf& b)
{
	const Rectf::Vertices v1 = a.GetVertices();
	const Rectf::Vertices v2 = b.GetVertices();

	//X1 = topLeft.x
	//X2 = bottomRight.x
	//Y1 = topLeft.y
	//Y2 = BottomRight.y

	//if (RectA.X1 < RectB.X2 && RectA.X2 > RectB.X1 &&
	//RectA.Y1 > RectB.Y2&& RectA.Y2 < RectB.Y1)
	return
		v1.topLeft.x		<	v2.bottomRight.x	&&
		v1.topLeft.y		>	v2.bottomRight.y	&&
		v1.bottomRight.x	>	v2.topLeft.x		&&
		v1.bottomRight.y	<	v2.topLeft.y;
}

bool rujin::collision::IsIntersecting(const glm::vec2& p1, const glm::vec2& p2, bool isLineSegment, const Rectf& r, float* pNearT, float* pFarT)
{
	// 4 floats to convert rect space to line space
	const float x1{ (r.left - p1.x) / (p2.x - p1.x) };
	const float x2{ (r.left + r.width - p1.x) / (p2.x - p1.x) };
	const float y1{ (r.bottom - p1.y) / (p2.y - p1.y) };
	const float y2{ (r.bottom + r.height - p1.y) / (p2.y - p1.y) };

	using std::max; using std::min;

	const float tNear{ max(min(x1,x2),min(y1,y2)) };
	const float tFar{ min(max(x1,x2), max(y1,y2)) };

	if (tNear > tFar)
		return false;

	if (isLineSegment && (tNear < 0.f || tNear > 1.f) && (tFar < 0.f || tFar > 1.f))
		return false;

	if (pNearT)
		*pNearT = tNear;
	if (pFarT) 
		*pFarT = tFar;

	return true;
}
