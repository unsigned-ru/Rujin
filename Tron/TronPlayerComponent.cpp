#include "TronPCH.h"
#include "TronPlayerComponent.h"

#include "BoxCollider.h"
#include "BoxColliderComponent.h"
#include "CollisionQuadTree.h"
#include "GameObject.h"
#include "InputEvents.h"
#include "InputService.h"
#include "RenderService.h"
#include "Scene.h"
#include "ServiceLocator.h"


TronPlayerComponent::TronPlayerComponent()
{
	InputService& input = ServiceLocator::GetService<InputService>();
	m_PlayerIdx = input.RegisterPlayer();

	input.AddAxisAction
	(
		m_PlayerIdx,
		static_cast<uint32_t>(AxisAction::MoveRight),
		AxisActionKeybinds{ KeyboardAxisKeybinds{'D', 'A'} }
	);

	input.AddAxisAction
	(
		m_PlayerIdx,
		static_cast<uint32_t>(AxisAction::MoveUp),
		AxisActionKeybinds{ KeyboardAxisKeybinds{'W', 'S'} }
	);
}

void TronPlayerComponent::Start()
{
	m_pBoxCollider = GameObject()->GetComponent<BoxColliderComponent>();
	assert(m_pBoxCollider); //This component requires a box collider component on the gameobject.

}

void TronPlayerComponent::FixedUpdate()
{
	const InputService& input = ServiceLocator::GetService<InputService>();
	CollisionQuadTree* pCollisionTree = GameObject()->GetScene()->GetCollisionQuadTree();

	HandleMovement(input, pCollisionTree);
}

void TronPlayerComponent::Draw() const
{
#ifdef _DEBUG 
	RenderService& rs = ServiceLocator::GetService<RenderService>();

	rs.SetColor({ 0.f, 1.f, 0.f, 1.f });
	rs.DrawPoint(GameObject()->GetTransform()->GetPosition(), 5.f);
	rs.SetColor();


	//ray 1
	{
		const Rectf::Vertices colliderVertices = static_cast<const BoxCollider*>(m_pBoxCollider->GetCollider())->GetRect().GetVertices();

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
		const Rectf::Vertices colliderVertices = static_cast<const BoxCollider*>(m_pBoxCollider->GetCollider())->GetRect().GetVertices();

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
		const Rectf::Vertices colliderVertices = static_cast<const BoxCollider*>(m_pBoxCollider->GetCollider())->GetRect().GetVertices();

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
		const Rectf::Vertices colliderVertices = static_cast<const BoxCollider*>(m_pBoxCollider->GetCollider())->GetRect().GetVertices();

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

void TronPlayerComponent::HandleMovement(const InputService& input, CollisionQuadTree* pCollisionTree)
{
	float inputIntensity;
	const float deltaTime = Rujin::Get()->GetFixedUpdateDeltaTime();
	TransformComponent* pTransform = m_pGameObject->GetTransform();

	if (input.IsAxisActionTriggered(0, static_cast<uint32_t>(AxisAction::MoveRight), &inputIntensity))
	{
		const Direction newDirection = inputIntensity > 0.f ? Direction::RIGHT : Direction::LEFT;

		//if we want to go right
		if (newDirection == Direction::RIGHT)
		{
			//if we aren't facing right.
			if (m_FacingDirection != Direction::RIGHT)
			{
				//raycast from bottomRight to topRight + 1 pixel to the right
				const Rectf::Vertices colliderVertices = static_cast<const BoxCollider*>(m_pBoxCollider->GetCollider())->GetRect().GetVertices();

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
				

				if (pCollisionTree->Raycast(p1, p2, { m_pBoxCollider->GetCollider() }, nullptr))
				{
					//there is a wall on our right...
					//move in the direction we were facing!
					glm::vec2 moveDir;
					switch (m_FacingDirection)
					{
					default:
						m_FacingDirection = Direction::UP;
						//intentional break-through.
					case Direction::UP:
						moveDir = { 0.f, 1.f };
						break;
					case Direction::DOWN:
						moveDir = { 0.f, -1.f };
						break;
					}

					//we hit a wall, keep going in the facing direction
					pTransform->AddLocalPosition(moveDir * abs(inputIntensity) * m_MoveSpeed * deltaTime);
				}
				else
				{
					//we can move to the right!
					//change our facing direction
					m_FacingDirection = newDirection;

					//move...
					pTransform->AddLocalPosition(glm::vec2{ inputIntensity * m_MoveSpeed * deltaTime,  0 });
				}
			}
			else 
			{
				//we are already facing right, just move...
				pTransform->AddLocalPosition(glm::vec2{ inputIntensity * m_MoveSpeed * deltaTime,  0 });
			}
		}
		//if we want to go left
		else if (newDirection == Direction::LEFT)
		{
			//if we aren't facing left
			if (m_FacingDirection != Direction::LEFT)
			{
				//raycast from bottomLeft to topleft + 1 pixel to the left
				const Rectf::Vertices colliderVertices = static_cast<const BoxCollider*>(m_pBoxCollider->GetCollider())->GetRect().GetVertices();

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

				if (pCollisionTree->Raycast(p1, p2, { m_pBoxCollider->GetCollider() }))
				{
					//there is a wall on our left
					//move in the direction we were facing!
					glm::vec2 moveDir;
					switch (m_FacingDirection)
					{
					default:
						m_FacingDirection = Direction::UP;
						//intentional break-through.
					case Direction::UP:
						moveDir = { 0.f, 1.f };
						break;
					case Direction::DOWN:
						moveDir = { 0.f, -1.f };
						break;
					}

					//keep moving in the facing direction!
					pTransform->AddLocalPosition(moveDir * abs(inputIntensity) * m_MoveSpeed * deltaTime);
				}
				else
				{
					//we can move to the left!
					//change our facing direction
					m_FacingDirection = newDirection;

					//move...
					pTransform->AddLocalPosition(glm::vec2{ inputIntensity * m_MoveSpeed * deltaTime,  0 });
				}
			}
			else
			{
				// we are already facing left
				pTransform->AddLocalPosition(glm::vec2{ inputIntensity * m_MoveSpeed * deltaTime,  0 });
			}
		}
	}
	else if (input.IsAxisActionTriggered(0, static_cast<uint32_t>(AxisAction::MoveUp), &inputIntensity))
	{
		const Direction newDirection = inputIntensity > 0.f ? Direction::UP : Direction::DOWN;

		//if we want to go up
		if (newDirection == Direction::UP)
		{
			//if we aren't facing up.
			if (m_FacingDirection != Direction::UP)
			{
				//raycast from topLeft to topRight + 1 pixel up.
				const Rectf::Vertices colliderVertices = static_cast<const BoxCollider*>(m_pBoxCollider->GetCollider())->GetRect().GetVertices();

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

				if (pCollisionTree->Raycast(p1, p2, { m_pBoxCollider->GetCollider() }, nullptr, nullptr))
				{
					//There is a wall above us...
					//move in the direction we were facing!
					glm::vec2 moveDir;
					switch (m_FacingDirection)
					{
					default:
						m_FacingDirection = Direction::RIGHT;
						//intentional break-through.
					case Direction::RIGHT:
						moveDir = { 1.f, 0.f};
						break;
					case Direction::LEFT:
						moveDir = { -1.f, 0.f};
						break;
					}

					//keep going in the facing direction
					pTransform->AddLocalPosition(moveDir * abs(inputIntensity) * m_MoveSpeed * deltaTime);
				}
				else
				{
					//we can move up!
					//change our facing direction
					m_FacingDirection = newDirection;

					//move...
					pTransform->AddLocalPosition(glm::vec2{ 0.f,  inputIntensity * m_MoveSpeed * deltaTime });
				}
			}
			else 
			{
				//we are already facing up, just move...
				pTransform->AddLocalPosition(glm::vec2{ 0.f, inputIntensity * m_MoveSpeed * deltaTime });
			}
		}
		//if we want to go down
		else if (newDirection == Direction::DOWN)
		{
			//if we aren't facing down
			if (m_FacingDirection != Direction::DOWN)
			{
				//raycast from bottomLeft to borromRight + 1 pixel down
				const Rectf::Vertices colliderVertices = static_cast<const BoxCollider*>(m_pBoxCollider->GetCollider())->GetRect().GetVertices();

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
				if (pCollisionTree->Raycast(p1, p2, { m_pBoxCollider->GetCollider() }))
				{
					//we have a wall below us...
					//move in the direction we were facing!
					glm::vec2 moveDir;
					switch (m_FacingDirection)
					{
					default:
						m_FacingDirection = Direction::RIGHT;
						//intentional break-through.
					case Direction::RIGHT:
						moveDir = { 1.f, 0.f};
						break;
					case Direction::LEFT:
						moveDir = { -1.f, 0.f };
						break;
					}

					//keep moving in the facing direction!
					pTransform->AddLocalPosition(moveDir * abs(inputIntensity) * m_MoveSpeed * deltaTime);
				}
				else
				{
					//we can move down!
					//change our facing direction
					m_FacingDirection = newDirection;

					//move...
					pTransform->AddLocalPosition(glm::vec2{ 0, inputIntensity * m_MoveSpeed * deltaTime });
				}
			}
			else
			{
				// we are already facing down
				// just move!
				pTransform->AddLocalPosition(glm::vec2{0, inputIntensity * m_MoveSpeed * deltaTime });
			}
		}
	}
}
