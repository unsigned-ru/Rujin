#include "TronPCH.h"
#include "RecognizerAIController.h"

#include "BoxColliderComponent.h"
#include "CollisionFunctions.h"
#include "GameObject.h"
#include "RecognizerComponent.h"
#include "Scene.h"
#include "TronMovementComponent.h"
#include "TronPlayerComponent.h"
#include "Rutils/String.h"

RecognizerAIController::RecognizerAIController(RecognizerComponent* pRecognizer)
	: m_pRecognizer(pRecognizer)
{
}

void RecognizerAIController::Start()
{
	EnemyAIController::Start();

	//find the players
	m_Players = GameObject()->GetScene()->GetAllRootGameObjectsByPredicate
	(
		[](rujin::GameObject* pObj)
		{
			return rutils::StartsWith(pObj->GetName(), "Player") && pObj->GetComponent<TronPlayerComponent>();
		}
	);
}

void RecognizerAIController::FixedUpdate()
{
	EnemyAIController::FixedUpdate();

	/* Sort players by distance ascending. idx 0 = closest player*/
	{
		const Position& currentPos = GameObject()->GetTransform().GetPosition();
		std::ranges::sort(m_Players, [&currentPos](rujin::GameObject* a, rujin::GameObject* b) { return distance2(currentPos, a->GetTransform().GetPosition()) < distance2(currentPos, b->GetTransform().GetPosition()); });
	}

	ExecuteCurrentState();
	HandleStateTransitions();
}

void RecognizerAIController::MoveToClosestPlayer()
{
	rujin::GameObject* pClosestPlayer = m_Players[0];

	//get pathfinding
	auto* pPathFinding = GameObject()->GetScene()->GetPathfinder();
	std::vector<graph::GridTerrainNode*> path = pPathFinding->FindPath
	(
		GameObject()->GetTransform().GetPosition(), pClosestPlayer->GetTransform().GetPosition()
	);

	const Rectf colliderRect = m_pRecognizer->GetColliderComponent()->GetCollider()->GetRect();

	//find first node that doesn't overlap our rect.
	const auto it = std::ranges::find_if
	(
		path,
		[&colliderRect, &pPathFinding](graph::GridTerrainNode* pNode)
		{
			return !collision::IsInside(pPathFinding->GetGraph()->GetNodeWorldPos(pNode), colliderRect);
		}
	);

	if (it == path.end())
		return;

	glm::vec2 moveDir = normalize(pPathFinding->GetGraph()->GetNodeWorldPos(*it) - GameObject()->GetTransform().GetPosition());

	std::array<std::pair<Direction, float>, 2> movementDirs;

	if (moveDir.x > 0.f)
		movementDirs[0] = std::make_pair(Direction::RIGHT, moveDir.x);
	else
		movementDirs[0] = std::make_pair(Direction::LEFT, abs(moveDir.x));

	if (moveDir.y > 0.f)
		movementDirs[1] = std::make_pair(Direction::UP, moveDir.y);
	else
		movementDirs[1] = std::make_pair(Direction::DOWN, abs(moveDir.y));

	//sort in descending order
	std::ranges::sort(movementDirs, [](const std::pair<Direction, float>& a, const std::pair<Direction, float>& b) { return a.second > b.second; });

	auto* pMovement = m_pRecognizer->GetMovement();

	for (const auto& dir : movementDirs)
	{
		if (pMovement->GetFacingDirection() == dir.first && !pMovement->CanMoveInDirection(dir.first))
			continue;

		switch (dir.first)
		{
			case Direction::LEFT:
			{
				pMovement->MoveLeft(1.f, Rujin::Get()->GetFixedUpdateDeltaTime());
				return;
			}
			case Direction::UP:
			{
				pMovement->MoveUp(1.f, Rujin::Get()->GetFixedUpdateDeltaTime());
				return;
			}
			case Direction::RIGHT:
			{
				pMovement->MoveRight(1.f, Rujin::Get()->GetFixedUpdateDeltaTime());
				return;
			}
			case Direction::DOWN:
			{
				pMovement->MoveDown(1.f, Rujin::Get()->GetFixedUpdateDeltaTime());
				return;
			}
		}
	}

	//we can't move in the direction we want to, execute the checked version
	switch (movementDirs[0].first)
	{
		case Direction::LEFT:
		{
			pMovement->MoveLeft(1.f, Rujin::Get()->GetFixedUpdateDeltaTime());
			break;
		}
		case Direction::UP:
		{
			pMovement->MoveUp(1.f, Rujin::Get()->GetFixedUpdateDeltaTime());
			break;
		}
		case Direction::RIGHT:
		{
			pMovement->MoveRight(1.f, Rujin::Get()->GetFixedUpdateDeltaTime());
			break;
		}
		case Direction::DOWN:
		{
			pMovement->MoveDown(1.f, Rujin::Get()->GetFixedUpdateDeltaTime());
			break;
		}
	}
}

void RecognizerAIController::ExecuteCurrentState()
{
	switch (m_State)
	{
	case State::MoveToClosestPlayer:
		MoveToClosestPlayer();
		break;
	case State::Attack:
		m_pRecognizer->Attack(m_Players[0]);
		break;
	}
}

void RecognizerAIController::HandleStateTransitions()
{
	switch (m_State)
	{
		case State::MoveToClosestPlayer:
		{
			if
			(
				IsInRange(m_Players[0], m_MaxAttackDistance) &&
				GetDirectionToObject(m_Players[0]) == m_pRecognizer->GetMovement()->GetFacingDirection() &&
				IsInLineOfSight(m_Players[0], { m_pRecognizer->GetColliderComponent()->GetCollider() })
			)
			{
				m_State = State::Attack;
			}
			break;
		}
		case State::Attack:
		{
			if
			(
				!IsInRange(m_Players[0], m_MaxAttackDistance) ||
				GetDirectionToObject(m_Players[0]) != m_pRecognizer->GetMovement()->GetFacingDirection() ||
				!IsInLineOfSight(m_Players[0], { m_pRecognizer->GetColliderComponent()->GetCollider() })
			)
			{
				m_State = State::MoveToClosestPlayer;
			}
		}
	}
}
