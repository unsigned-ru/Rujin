#include "TronPCH.h"
#include "TankAIController.h"

#include <ranges>

#include "CollisionFunctions.h"
#include "GameObject.h"
#include "RenderService.h"
#include "Scene.h"
#include "ServiceLocator.h"
#include "TankComponent.h"
#include "TankMovementComponent.h"
#include "TronPlayerComponent.h"
#include "Rutils/String.h"

TankAIController::TankAIController(TankComponent* pTank)
	: m_pTank(pTank)
{}

void TankAIController::Start()
{
	Component::Start();

	//find the players
	m_Players = GameObject()->GetScene()->GetAllRootGameObjectsByPredicate
	(
		[](rujin::GameObject* pObj)
		{
			return rutils::StartsWith(pObj->GetName(), "Player") && pObj->GetComponent<TronPlayerComponent>();
		}
	);
}

void TankAIController::FixedUpdate()
{
	Component::FixedUpdate();

	HandleMovement();
}

void TankAIController::Draw() const
{
	Component::Draw();

	auto& rs = ServiceLocator::GetService<RenderService>();
	rs.SetColor({ 0.f, 1.f, 1.f, 1.f });
	for (auto* node : m_Path)
	{
		auto pos = static_cast<AStar<graph::GridTerrainNode, graph::GraphConnection>*>(GameObject()->GetScene()->GetPathfinder())->GetGraph()->GetNodeWorldPos(node);
		
		rs.DrawPoint(pos, 10);
	}
	rs.SetColor();
}

void TankAIController::HandleMovement()
{
	//get closest player
	//sort by distance
	const Position & currentPos = GameObject()->GetTransform().GetPosition();
	std::ranges::sort(m_Players, [&currentPos](rujin::GameObject* a, rujin::GameObject* b) { return distance2(currentPos, a->GetTransform().GetPosition()) < distance2(currentPos, b->GetTransform().GetPosition()); });

	rujin::GameObject* pClosestPlayer = m_Players[0];

	//get pathfinding
	auto* pPathFinding = GameObject()->GetScene()->GetPathfinder();
	m_Path = pPathFinding->FindPath(currentPos, pClosestPlayer->GetTransform().GetPosition());

	const Rectf colliderRect = m_pTank->GetColliderComponent()->GetCollider()->GetRect();

	//find first node that doesn't overlap our rect.
	auto it = std::find_if
	(
		m_Path.begin(),
		m_Path.end(),
		[&colliderRect, &pPathFinding](graph::GridTerrainNode* pNode)
		{
			return !collision::IsInside(pPathFinding->GetGraph()->GetNodeWorldPos(pNode), colliderRect);
		}
	);

	if (it == m_Path.end())
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
