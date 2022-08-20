#ifndef TANK_AI_CONTROLLER_H
#define TANK_AI_CONTROLLER_H
#include "Component.h"
#include "EnemyAIController.h"

namespace rujin::graph
{
	class GridTerrainNode;
}

class TankComponent;

class TankAIController final : public EnemyAIController, public event::IObserver
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

	void OnNotify(const uint32_t identifier, const event::Data* pEventData) override;

	State m_State{ State::MoveToClosestPlayer };
	std::vector<rujin::GameObject*> m_Players{};

	TankComponent* m_pTank = nullptr;

	const float m_MaxShootDistance{ 700.f };

	inline static constexpr uint32_t s_Score{ 100u };
};


#endif // Include Guard: TANK_AI_CONTROLLER_H
