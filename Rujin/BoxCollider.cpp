#include "RujinPCH.h"
#include "BoxCollider.h"

#include "ColliderComponent.h"
#include "GameObject.h"
#include "CollisionFunctions.h"

rujin::BoxCollider::BoxCollider(ColliderComponent* pComponent, const glm::vec2& size, const bool isStatic, const glm::vec2& pivot)
	: Collider(pComponent, isStatic, pivot)
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
	const TransformComponent* pTransform = m_pComponent->GameObject()->GetTransform();
	const Position pos = pTransform->GetPosition();
	//TODO: add rotation.
	//const Rotation rot = pTransform->GetRotation();
	const Scale scale = pTransform->GetScale();

	rv.left = pos.x - (m_Pivot.x * m_Size.x * scale.x);
	rv.bottom = pos.y - (m_Pivot.y * m_Size.y * scale.y);
	rv.width = m_Size.x * scale.x;
	rv.height = m_Size.y * scale.y;

	return rv;
}

rujin::CollisionResult rujin::BoxCollider::IsOverlapping(const Collider* pOther)
{
	CollisionResult rv{};

	if (const BoxCollider* pBoxCollider = dynamic_cast<const BoxCollider*>(pOther); pBoxCollider)
	{
		//get vertices
		auto a = GetRect().GetVertices();
		auto b = pBoxCollider->GetRect().GetVertices();

		//check if overlapping.
		if (collision::IsOverlapping(GetRect(), pBoxCollider->GetRect()))
		{
			//we are overlapping.
			rv.isColliding = true;
			rv.other = pOther;
		}
	}
	else
		LOG_WARNING("Unimplemented Collider detected. Did you forget to implement?");

	return rv;
}

void rujin::BoxCollider::ResolveOverlap(const CollisionResult& result)
{
	if (m_IsStatic || !result.isColliding) return;

	TransformComponent* pTransform = m_pComponent->GameObject()->GetTransform();

	const Rectf a = GetRect();

	if (const BoxCollider* pOtherBox = dynamic_cast<const BoxCollider*>(result.other); pOtherBox)
	{
		const Rectf b = pOtherBox->GetRect();

		float resolve{};

		const float xDiff = (a.left + (a.width / 2.f)) - (b.left + (b.width / 2.f));
		const float yDiff = (a.bottom + (a.height / 2.f)) - (b.bottom + (b.height / 2.f));

		//find longest resolve axis.
		if (abs(xDiff) > abs(yDiff)) //longest axis is X
		{
			if(xDiff > 0) // colliding on our left
			{
				// We add a positive x value to move the object to the right.
				resolve = b.left + b.width - a.left;
			}
			else //colliding on our right.
			{
				// We add a negative x value to move the object to the left.
				resolve = b.left - (a.left + a.width);
			}

			pTransform->AddLocalPosition({ resolve, 0 });
		}
		else //longest axis is Y
		{
			if (yDiff > 0) // colliding on our bottom
			{
				// We add a negative y value to move the object towards the bottom.
				resolve = b.bottom + b.height - a.bottom;
			}
			else //colliding on our bottom.
			{
				// We add a positive y value to move the object towards the top.
				resolve = b.bottom - (a.bottom + a.height);
			}

			pTransform->AddLocalPosition({ 0, resolve });
		}
	}
	else
		LOG_WARNING("Unimplemented Collider detected. Did you forget to implement?");

}
