#include "TronPCH.h"
#include "TankAIController.h"

#include <ranges>

#include "CollisionFunctions.h"
#include "GameEvents.h"
#include "GameObject.h"
#include "HealthComponent.h"
#include "Scene.h"
#include "TankComponent.h"
#include "TronMovementComponent.h"
#include "TronPlayerComponent.h"
#include "Rutils/String.h"



TankAIController::TankAIController(TankComponent* pTank)
	: m_pTank(pTank)
{}

void TankAIController::Start()
{
	EnemyAIController::Start();

	FindPlayers();
}

void TankAIController::FixedUpdate()
{
	EnemyAIController::FixedUpdate();

	if (m_Players.empty())
	{
		FindPlayers();
		return;
	}

	/* Sort players by distance ascending. idx 0 = closest player*/
	{
		const Position& currentPos = GameObject()->GetTransform().GetPosition();
		std::ranges::sort(m_Players, [&currentPos](rujin::GameObject* a, rujin::GameObject* b) { return distance2(currentPos, a->GetTransform().GetPosition()) < distance2(currentPos, b->GetTransform().GetPosition()); });
	}

	ExecuteCurrentState();
	HandleStateTransitions();
}

void TankAIController::FindPlayers()
{
	//find the players
	m_Players = GameObject()->GetScene()->GetAllRootGameObjectsByPredicate
	(
		[](rujin::GameObject* pObj)
		{
			return rutils::StartsWith(pObj->GetName(), "Player") && pObj->GetComponent<TronPlayerComponent>();
		}
	);

	m_pTank->GetHealthComponent()->AddObserver(this);
}

void TankAIController::ExecuteCurrentState()
{
	switch (m_State)
	{
	case State::MoveToClosestPlayer:
		MoveToClosestPlayer();
		break;
	case State::Shoot:
		m_pTank->Shoot();
		break;
	}
}

void TankAIController::HandleStateTransitions()
{
	switch (m_State)
	{
		case State::MoveToClosestPlayer:
		{
			if
			(
				IsInRange(m_Players[0], m_MaxShootDistance) &&
				GetDirectionToObject(m_Players[0]) == m_pTank->GetMovement()->GetFacingDirection() &&
				IsInLineOfSight(m_Players[0], { m_pTank->GetColliderComponent()->GetCollider() })
			)
			{
				m_State = State::Shoot;
			}
			break;
		}
		case State::Shoot:
		{
			if
			(
				!IsInRange(m_Players[0], m_MaxShootDistance) ||
				GetDirectionToObject(m_Players[0]) != m_pTank->GetMovement()->GetFacingDirection() ||
				!IsInLineOfSight(m_Players[0], { m_pTank->GetColliderComponent()->GetCollider() })
			)
			{
				m_State = State::MoveToClosestPlayer;
			}
		}
	}
}

void TankAIController::OnNotify(const uint32_t identifier, const event::Data* pEventData)
{
	if (identifier == static_cast<uint32_t>(game_event::Identifier::OnDie))
	{
		const auto* pOnDieEvent = static_cast<const game_event::OnDie_t*>(pEventData);

		//if we got killed by a player, hand the player the score
		if (pOnDieEvent->pKilledBy && pOnDieEvent->pKilledBy->HasTag("Player"))
		{
			if (TronPlayerComponent* pPlayer = pOnDieEvent->pKilledBy->GetComponent<TronPlayerComponent>(); pPlayer)
			{
				pPlayer->AddScore(s_Score);
			}
		}


		//destroy our root game object
		GameObject()->GetRootParent()->Destroy();
	}
}

void TankAIController::MoveToClosestPlayer()
{
	rujin::GameObject* pClosestPlayer = m_Players[0];

	//get pathfinding
	auto* pPathFinding = GameObject()->GetScene()->GetPathfinder();
	std::vector<graph::GridTerrainNode*> path = pPathFinding->FindPath
	(
		GameObject()->GetTransform().GetPosition(), pClosestPlayer->GetTransform().GetPosition()
	);

	const Rectf colliderRect = m_pTank->GetColliderComponent()->GetCollider()->GetRect();

	//find first node that doesn't overlap our rect.
	auto it = std::ranges::find_if
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

	auto* pTankMovement = m_pTank->GetMovement();

	for (const auto& dir : movementDirs)
	{
		if (pTankMovement->GetFacingDirection() == dir.first && !pTankMovement->CanMoveInDirection(dir.first))
			continue;

		switch (dir.first)
		{
		case Direction::LEFT:
		{
			pTankMovement->MoveLeft(1.f, Rujin::Get()->GetFixedUpdateDeltaTime());
			return;
		}
		case Direction::UP:
		{
			pTankMovement->MoveUp(1.f, Rujin::Get()->GetFixedUpdateDeltaTime());
			return;
		}
		case Direction::RIGHT:
		{
			pTankMovement->MoveRight(1.f, Rujin::Get()->GetFixedUpdateDeltaTime());
			return;
		}
		case Direction::DOWN:
		{
			pTankMovement->MoveDown(1.f, Rujin::Get()->GetFixedUpdateDeltaTime());
			return;
		}
		}
	}

	//we can't move in the direction we want to, execute the checked version
	switch (movementDirs[0].first)
	{
	case Direction::LEFT:
	{
		pTankMovement->MoveLeft(1.f, Rujin::Get()->GetFixedUpdateDeltaTime());
		break;
	}
	case Direction::UP:
	{
		pTankMovement->MoveUp(1.f, Rujin::Get()->GetFixedUpdateDeltaTime());
		break;
	}
	case Direction::RIGHT:
	{
		pTankMovement->MoveRight(1.f, Rujin::Get()->GetFixedUpdateDeltaTime());
		break;
	}
	case Direction::DOWN:
	{
		pTankMovement->MoveDown(1.f, Rujin::Get()->GetFixedUpdateDeltaTime());
		break;
	}
	}
}
