#ifndef TANK_AI_CONTROLLER_H
#define TANK_AI_CONTROLLER_H
#include "Component.h"
#include "EnemyAIController.h"

namespace rujin::graph
{
	class GridTerrainNode;
}

class TankComponent;

class TankAIController final : public EnemyAIController
{
public:
	explicit TankAIController(TankComponent* pTank);

	void Start() override;
	void FixedUpdate() override;

	enum class State
	{
		MoveToClosestPlayer,
		Shoot
	};

private:
	void MoveToClosestPlayer();

	void ExecuteCurrentState();
	void HandleStateTransitions();

	State m_State{ State::MoveToClosestPlayer };
	std::vector<rujin::GameObject*> m_Players{};

	TankComponent* m_pTank = nullptr;

	const float m_MaxShootDistance{ 700.f };
};


#endif // Include Guard: TANK_AI_CONTROLLER_H
