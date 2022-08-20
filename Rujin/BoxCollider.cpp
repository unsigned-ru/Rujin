#include "RujinPCH.h"
#include "BoxCollider.h"

#include "BoxColliderComponent.h"
#include "GameObject.h"
#include "CollisionFunctions.h"

rujin::BoxCollider::BoxCollider(BoxColliderComponent* pComponent, const glm::vec2& size, const bool isStatic, const glm::vec2& pivot, CollisionLayer collisionLayer)
	: Collider(pComponent, isStatic, pivot, collisionLayer)
	, m_Size(size)
{
}

const glm::vec2& rujin::BoxCollider::GetSize() const
{
	return m_Size;
}

void rujin::BoxCollider::SetSize(const glm::vec2& size)
{
	m_Size = size;
}

rujin::Rectf rujin::BoxCollider::GetRect() const
{
	//generate rect from transform, pivot and size.
	Rectf rv;
	const Transform& pTransform = m_pComponent->GameObject()->GetTransform();
	const Position pos = pTransform.GetPosition();
	//TODO: add rotation.
	//const Rotation rot = pTransform->GetRotation();
	const Scale scale = pTransform.GetScale();

	rv.left = pos.x - (m_Pivot.x * m_Size.x * scale.x);
	rv.bottom = pos.y - (m_Pivot.y * m_Size.y * scale.y);
	rv.width = m_Size.x * scale.x;
	rv.height = m_Size.y * scale.y;

	return rv;
}

rujin::CollisionResult rujin::BoxCollider::IsOverlapping(const Collider* pOther) const
{
	CollisionResult rv{};
	rv.self = this;
	rv.other = pOther;

	const Rectf a = GetRect();
	if (const auto* pOtherBox = dynamic_cast<const BoxCollider*>(pOther); pOtherBox)
	{
		const Rectf b = pOtherBox->GetRect();

		const float xDiff = (a.left + (a.width / 2.f)) - (b.left + (b.width / 2.f));
		const float yDiff = (a.bottom + (a.height / 2.f)) - (b.bottom + (b.height / 2.f));
		const float averageWidth = (a.width + b.width) / 2.f;
		const float averageHeight = (a.height + b.height) / 2.f;

		//if either distance is greater than the average dimension there is no collision.
		if (abs(xDiff) > averageWidth || abs(yDiff) > averageHeight)
		{
			//no collision
			rv.isColliding = false;
			return rv;
		}

		rv.isColliding = true;

		//to determine which region of this rectangle the rect's center point is in,
		//we have to account for the scale of this rectangle.
		//To do that, we divide dx and dy by its width and height respectively.
		if (abs(xDiff / b.width) > abs(yDiff / b.height))
		{
			if (xDiff > 0) // colliding on our left
				rv.collisionDirection = Direction::LEFT;
			else //colliding on our right.
				rv.collisionDirection = Direction::RIGHT;
		}
		else //longest axis is Y
		{
			if (yDiff > 0) // colliding on our bottom
			{
				rv.collisionDirection = Direction::DOWN;
			}
			else //colliding on our top.
			{
				rv.collisionDirection = Direction::UP;
			}
		}
	}
	else
		LOG_WARNING("Unimplemented Collider detected. Did you forget to implement?");

	return rv;
}

void rujin::BoxCollider::ResolveOverlap(const CollisionResult& result) const
{
	if (m_IsStatic || !result.isColliding) return;

	Transform& transform = m_pComponent->GameObject()->GetTransform();
	const Rectf a = GetRect();

	if (const BoxCollider* pOtherBox = dynamic_cast<const BoxCollider*>(result.other); pOtherBox)
	{
		const Rectf b = pOtherBox->GetRect();

		glm::vec2 resolve{};

		switch (result.collisionDirection)
		{
		case Direction::LEFT:
			resolve.x = b.left + b.width - a.left;
			break;
		case Direction::UP:
			resolve.y = b.bottom - (a.bottom + a.height);
			break;
		case Direction::RIGHT:
			resolve.x = b.left - (a.left + a.width);
			break;
		case Direction::DOWN:
			resolve.y = b.bottom + b.height - a.bottom;
			break;
		}

		transform.AddLocalPosition(resolve);
	}
	else
		LOG_WARNING("Unimplemented Collider detected. Did you forget to implement?");

}
