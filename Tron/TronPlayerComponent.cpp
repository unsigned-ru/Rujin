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
		const glm::vec2 offset = glm::vec2{ m_WallCheckOffset, 0.f };
		const Rectf::Vertices colliderVertices = static_cast<const BoxCollider*>(m_pBoxCollider->GetCollider())->GetRect().GetVertices();
		const glm::vec2 p1 = colliderVertices.bottomRight + offset;
		const glm::vec2 p2 = colliderVertices.topRight + offset;

		rs.DrawLine(p1, p2, 8.f);
	}

	//ray 2
	{
		const glm::vec2 offset = glm::vec2{ -m_WallCheckOffset, 0.f };
		const Rectf::Vertices colliderVertices = static_cast<const BoxCollider*>(m_pBoxCollider->GetCollider())->GetRect().GetVertices();
		const glm::vec2 p1 = colliderVertices.bottomLeft + offset;
		const glm::vec2 p2 = colliderVertices.topLeft + offset;

		rs.DrawLine(p1, p2, 8.f);
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
				//raycast from bottomRight to topRight + 1 pixel to the right, move forward until we don't have a wall on our Right.
				const glm::vec2 offset = glm::vec2{ m_WallCheckOffset, 0.f };
				const Rectf::Vertices colliderVertices = static_cast<const BoxCollider*>(m_pBoxCollider->GetCollider())->GetRect().GetVertices();
				const glm::vec2 p1 = colliderVertices.bottomRight + offset;
				const glm::vec2 p2 = colliderVertices.topRight + offset;

				if (pCollisionTree->Raycast(p1, p2, { m_pBoxCollider->GetCollider() }))
				{
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

					//we hit a wall, keep moving forward.
					pTransform->AddLocalPosition(moveDir * inputIntensity * m_MoveSpeed * deltaTime);
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
			else ////if we are facing right, we can just move.
			{
				//move...
				pTransform->AddLocalPosition(glm::vec2{ inputIntensity * m_MoveSpeed * deltaTime,  0 });
			}
		}
		//if we want to go left
		else if (newDirection == Direction::LEFT)
		{
			//if we aren't facing left
			if (m_FacingDirection != Direction::LEFT)
			{
				//raycast from bottomLeft to topleft + 1 pixel to the left, move forward until we don't have a wall on our left.
				const glm::vec2 offset = glm::vec2{ -m_WallCheckOffset, 0.f };
				const Rectf::Vertices colliderVertices = static_cast<const BoxCollider*>(m_pBoxCollider->GetCollider())->GetRect().GetVertices();
				const glm::vec2 p1 = colliderVertices.bottomLeft + offset;
				const glm::vec2 p2 = colliderVertices.topLeft + offset;

				if (pCollisionTree->Raycast(p1, p2, { m_pBoxCollider->GetCollider() }))
				{
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

					//we hit a wall, keep moving forward.
					pTransform->AddLocalPosition(moveDir * inputIntensity * m_MoveSpeed * deltaTime);
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
			else //if we are facing left, we can just move.
			{
				//move...
				pTransform->AddLocalPosition(glm::vec2{ inputIntensity * m_MoveSpeed * deltaTime,  0 });
			}
		}
	}
	else if (input.IsAxisActionTriggered(0, static_cast<uint32_t>(AxisAction::MoveUp), &inputIntensity))
	{
		const Direction newDirection = inputIntensity > 0.f ? Direction::UP : Direction::DOWN;
		m_FacingDirection = newDirection;
		pTransform->AddLocalPosition({ 0,  inputIntensity * m_MoveSpeed * deltaTime });
	}
}
