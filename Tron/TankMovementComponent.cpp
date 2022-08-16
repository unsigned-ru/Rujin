#include "TronPCH.h"
#include "TankMovementComponent.h"

#include "BoxCollider.h"
#include "BoxColliderComponent.h"
#include "CollisionQuadTree.h"
#include "GameObject.h"
#include "RenderService.h"
#include "Scene.h"
#include "ServiceLocator.h"
#include "TankComponent.h"


void TankMovementComponent::Start()
{
	ASSERT_MSG(m_pTank, "This component needs a TankComponent.");
}

void TankMovementComponent::Move(Direction dir, float inputIntensity, float deltaTime)
{
	switch (dir)
	{
		case Direction::LEFT:
			MoveLeft(inputIntensity, deltaTime);
			break;
		case Direction::UP:
			MoveUp(inputIntensity, deltaTime);
			break;
		case Direction::RIGHT:
			MoveRight(inputIntensity, deltaTime);
			break;
		case Direction::DOWN:
			MoveDown(inputIntensity, deltaTime);
			break;
	}
}

void TankMovementComponent::MoveRight(const float inputIntensity, const float deltaTime)
{
	//if we aren't facing right.
	if (m_FacingDirection != Direction::RIGHT)
	{
		if (!CanMoveRight())
		{
			//there is a wall on our right...
			//move in the direction we were facing!
			glm::vec2 moveDir;
			switch (m_FacingDirection)
			{
			default:
				SetFacingDirection(Direction::UP);
				//intentional break-through.
			case Direction::UP:
				moveDir = { 0.f, 1.f };
				break;
			case Direction::DOWN:
				moveDir = { 0.f, -1.f };
				break;
			}

			//we hit a wall, keep going in the facing direction
			GameObject()->GetTransform().AddLocalPosition(moveDir * abs(inputIntensity) * m_MoveSpeed * deltaTime);
		}
		else
		{
			MoveRight_Unchecked(inputIntensity, deltaTime);
		}
	}
	else
	{
		MoveRight_Unchecked(inputIntensity, deltaTime);
	}
}

void TankMovementComponent::MoveLeft(const float inputIntensity, const float deltaTime)
{
	//if we aren't facing left
	if (m_FacingDirection != Direction::LEFT)
	{
		if (!CanMoveLeft())
		{
			//there is a wall on our left
			//move in the direction we were facing!
			glm::vec2 moveDir;
			switch (m_FacingDirection)
			{
			default:
				SetFacingDirection(Direction::UP);
				//intentional break-through.
			case Direction::UP:
				moveDir = { 0.f, 1.f };
				break;
			case Direction::DOWN:
				moveDir = { 0.f, -1.f };
				break;
			}

			//keep moving in the facing direction!
			GameObject()->GetTransform().AddLocalPosition(moveDir * inputIntensity * m_MoveSpeed * deltaTime);
		}
		else
		{
			//we can move to the left!
			MoveLeft_Unchecked(inputIntensity, deltaTime);
		}
	}
	else
	{
		MoveLeft_Unchecked(inputIntensity, deltaTime);
	}
}

void TankMovementComponent::MoveUp(const float inputIntensity, const float deltaTime)
{
	//if we aren't facing up.
	if (m_FacingDirection != Direction::UP)
	{
		if (!CanMoveUp())
		{
			//There is a wall above us...
			//move in the direction we were facing!
			glm::vec2 moveDir;
			switch (m_FacingDirection)
			{
			default:
				SetFacingDirection(Direction::RIGHT);
				//intentional break-through.
			case Direction::RIGHT:
				moveDir = { 1.f, 0.f };
				break;
			case Direction::LEFT:
				moveDir = { -1.f, 0.f };
				break;
			}

			//keep going in the facing direction
			GameObject()->GetTransform().AddLocalPosition(moveDir * inputIntensity * m_MoveSpeed * deltaTime);
		}
		else
		{
			//we can move up!
			MoveUp_Unchecked(inputIntensity, deltaTime);
		}
	}
	else
	{
		//we are already facing up, just move...
		MoveUp_Unchecked(inputIntensity, deltaTime);
	}
}

void TankMovementComponent::MoveDown(const float inputIntensity, const float deltaTime)
{
	//if we aren't facing down
	if (m_FacingDirection != Direction::DOWN)
	{
		if (!CanMoveDown())
		{
			//we have a wall below us...
			//move in the direction we were facing!
			glm::vec2 moveDir;
			switch (m_FacingDirection)
			{
			default:
				SetFacingDirection(Direction::RIGHT);
				//intentional break-through.
			case Direction::RIGHT:
				moveDir = { 1.f, 0.f };
				break;
			case Direction::LEFT:
				moveDir = { -1.f, 0.f };
				break;
			}

			//keep moving in the facing direction!
			GameObject()->GetTransform().AddLocalPosition(moveDir * inputIntensity * m_MoveSpeed * deltaTime);
		}
		else
		{
			//we can move down!
			//change our facing direction
			MoveDown_Unchecked(inputIntensity, deltaTime);
		}
	}
	else
	{
		// we are already facing down
		// just move!
		MoveDown_Unchecked(inputIntensity, deltaTime);
	}
}

void TankMovementComponent::Move_Unchecked(Direction dir, float inputIntensity, float deltaTime)
{
	switch (dir)
	{
	case Direction::LEFT:
		MoveLeft_Unchecked(inputIntensity, deltaTime);
		break;
	case Direction::UP:
		MoveUp_Unchecked(inputIntensity, deltaTime);
		break;
	case Direction::RIGHT:
		MoveRight_Unchecked(inputIntensity, deltaTime);
		break;
	case Direction::DOWN:
		MoveDown_Unchecked(inputIntensity, deltaTime);
		break;
	}
}


void TankMovementComponent::MoveRight_Unchecked(const float inputIntensity, const float deltaTime)
{
	SetFacingDirection(Direction::RIGHT);
	GameObject()->GetTransform().AddLocalPosition(glm::vec2{ inputIntensity * m_MoveSpeed * deltaTime,  0 });
}

void TankMovementComponent::MoveLeft_Unchecked(const float inputIntensity, const float deltaTime)
{
	SetFacingDirection(Direction::LEFT);
	GameObject()->GetTransform().AddLocalPosition(glm::vec2{ -inputIntensity * m_MoveSpeed * deltaTime,  0 });
}

void TankMovementComponent::MoveUp_Unchecked(const float inputIntensity, const float deltaTime)
{
	SetFacingDirection(Direction::UP);
	GameObject()->GetTransform().AddLocalPosition(glm::vec2{ 0.f,  inputIntensity * m_MoveSpeed * deltaTime });
}

void TankMovementComponent::MoveDown_Unchecked(const float inputIntensity, const float deltaTime)
{
	SetFacingDirection(Direction::DOWN);
	GameObject()->GetTransform().AddLocalPosition(glm::vec2{ 0, -inputIntensity * m_MoveSpeed * deltaTime });
}


bool TankMovementComponent::CanMoveInDirection(Direction dir) const
{
	switch (dir)
	{
	case Direction::LEFT:
		return CanMoveLeft();
	case Direction::UP:
		return CanMoveUp();
	case Direction::RIGHT:
		return CanMoveRight();
	case Direction::DOWN:
		return CanMoveDown();
	}

	return false;
}

bool TankMovementComponent::CanMoveRight() const
{
	//raycast from bottomRight to topRight + 1 pixel to the right
	const BoxCollider* pCollider = m_pTank->GetColliderComponent()->GetCollider();
	const Rectf::Vertices colliderVertices = pCollider->GetRect().GetVertices();

	const glm::vec2 p1
	{
		colliderVertices.bottomRight.x + s_WallCheckOffset,
		colliderVertices.bottomRight.y + s_WallCheckLengthReduction
	};

	const glm::vec2 p2
	{
		colliderVertices.topRight.x + s_WallCheckOffset,
		colliderVertices.topRight.y - s_WallCheckLengthReduction
	};

	return !GameObject()->GetScene()->GetCollisionQuadTree()->Raycast(p1, p2, { pCollider }, nullptr);
}

bool TankMovementComponent::CanMoveLeft() const
{
	//raycast from bottomLeft to topleft + 1 pixel to the left
	const BoxCollider* pCollider = m_pTank->GetColliderComponent()->GetCollider();
	const Rectf::Vertices colliderVertices = pCollider->GetRect().GetVertices();

	const glm::vec2 p1
	{
		colliderVertices.bottomLeft.x - s_WallCheckOffset,
		colliderVertices.bottomLeft.y + s_WallCheckLengthReduction
	};

	const glm::vec2 p2
	{
		colliderVertices.topLeft.x - s_WallCheckOffset,
		colliderVertices.topLeft.y - s_WallCheckLengthReduction
	};

	return !GameObject()->GetScene()->GetCollisionQuadTree()->Raycast(p1, p2, { pCollider }, nullptr);
}

bool TankMovementComponent::CanMoveUp() const
{
	//raycast from topLeft to topRight + 1 pixel up.
	const BoxCollider* pCollider = m_pTank->GetColliderComponent()->GetCollider();
	const Rectf::Vertices colliderVertices = pCollider->GetRect().GetVertices();

	const glm::vec2 p1
	{
		colliderVertices.topLeft.x + s_WallCheckLengthReduction,
		colliderVertices.topLeft.y + s_WallCheckOffset
	};

	const glm::vec2 p2
	{
		colliderVertices.topRight.x - s_WallCheckLengthReduction,
		colliderVertices.topRight.y + s_WallCheckOffset
	};

	return !GameObject()->GetScene()->GetCollisionQuadTree()->Raycast(p1, p2, { pCollider }, nullptr);
}

bool TankMovementComponent::CanMoveDown() const
{
	//raycast from bottomLeft to borromRight + 1 pixel down
	const BoxCollider* pCollider = m_pTank->GetColliderComponent()->GetCollider();
	const Rectf::Vertices colliderVertices = pCollider->GetRect().GetVertices();

	const glm::vec2 p1
	{
		colliderVertices.bottomLeft.x + s_WallCheckLengthReduction,
		colliderVertices.bottomLeft.y - s_WallCheckOffset
	};
	const glm::vec2 p2 =
	{
		colliderVertices.bottomRight.x - s_WallCheckLengthReduction,
		colliderVertices.bottomRight.y - s_WallCheckOffset
	};

	return !GameObject()->GetScene()->GetCollisionQuadTree()->Raycast(p1, p2, { pCollider }, nullptr);
}

Direction TankMovementComponent::GetFacingDirection() const
{
	return m_FacingDirection;
}


void TankMovementComponent::Draw() const
{
#ifdef _DEBUG 
	RenderService& rs = ServiceLocator::GetService<RenderService>();
	//ray 1
	{
		const Rectf::Vertices colliderVertices = m_pTank->GetColliderComponent()->GetCollider()->GetRect().GetVertices();

		const glm::vec2 p1
		{
			colliderVertices.bottomRight.x + s_WallCheckOffset,
			colliderVertices.bottomRight.y + s_WallCheckLengthReduction
		};

		const glm::vec2 p2
		{
			colliderVertices.topRight.x + s_WallCheckOffset,
			colliderVertices.topRight.y - s_WallCheckLengthReduction
		};

		rs.DrawLine(p1, p2, 3.f);
	}

	//ray 2
	{
		const Rectf::Vertices colliderVertices = m_pTank->GetColliderComponent()->GetCollider()->GetRect().GetVertices();

		const glm::vec2 p1
		{
			colliderVertices.bottomLeft.x - s_WallCheckOffset,
			colliderVertices.bottomLeft.y + s_WallCheckLengthReduction
		};

		const glm::vec2 p2
		{
			colliderVertices.topLeft.x - s_WallCheckOffset,
			colliderVertices.topLeft.y - s_WallCheckLengthReduction
		};

		rs.DrawLine(p1, p2, 3.f);
	}

	//ray3
	{
		const Rectf::Vertices colliderVertices = m_pTank->GetColliderComponent()->GetCollider()->GetRect().GetVertices();

		const glm::vec2 p1
		{
			colliderVertices.topLeft.x + s_WallCheckLengthReduction,
			colliderVertices.topLeft.y + s_WallCheckOffset
		};

		const glm::vec2 p2
		{
			colliderVertices.topRight.x - s_WallCheckLengthReduction,
			colliderVertices.topRight.y + s_WallCheckOffset
		};

		rs.DrawLine(p1, p2, 3.f);
	}

	//ray4
	{
		const Rectf::Vertices colliderVertices = m_pTank->GetColliderComponent()->GetCollider()->GetRect().GetVertices();

		const glm::vec2 p1
		{
			colliderVertices.bottomLeft.x + s_WallCheckLengthReduction,
			colliderVertices.bottomLeft.y - s_WallCheckOffset
		};
		const glm::vec2 p2 =
		{
			colliderVertices.bottomRight.x - s_WallCheckLengthReduction,
			colliderVertices.bottomRight.y - s_WallCheckOffset
		};

		rs.DrawLine(p1, p2, 3.f);
	}

#endif
}

void TankMovementComponent::SetFacingDirection(Direction newDirection)
{
	if (m_FacingDirection == newDirection)
		return;

	m_FacingDirection = newDirection;

	Rotation rotation{};
	switch (newDirection)
	{
		case Direction::RIGHT:
			rotation = 0.f;
			break;
	case Direction::UP:
			rotation = std::numbers::pi_v<Rotation> / 2;
			break;
		case Direction::LEFT:
			rotation = std::numbers::pi_v<Rotation>;
			break;
		case Direction::DOWN:
			rotation = (std::numbers::pi_v<Rotation> / 2) * 3;
	}

	GameObject()->GetTransform().SetLocalRotation(rotation);
}

void TankMovementComponent::SetTank(TankComponent* pTank)
{
	m_pTank = pTank;
}
