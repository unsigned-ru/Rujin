#include "RujinPCH.h"
#include "CollisionFunctions.h"

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

	return
		v1.topLeft.x		<	v2.bottomRight.x	&&
		v1.topLeft.y		<	v2.bottomRight.y	&&
		v1.bottomRight.x	>	v2.topLeft.x		&&
		v1.bottomRight.y	>	v2.topLeft.y;
}
