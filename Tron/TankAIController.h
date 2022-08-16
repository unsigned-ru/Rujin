#ifndef TANK_AI_CONTROLLER_H
#define TANK_AI_CONTROLLER_H
#include "Component.h"
#include <vector>

namespace rujin
{
	namespace graph
	{
		class GridTerrainNode;
	}
}

class TankComponent;

class TankAIController : public rujin::Component
{
public:
	explicit TankAIController(TankComponent* pTank);

	void Start() override;
	void FixedUpdate() override;
	void Draw() const override;

private:

	void HandleMovement();

	std::vector<rujin::GameObject*> m_Players{};
	TankComponent* m_pTank = nullptr;

	std::vector<graph::GridTerrainNode*> m_Path{};
};


#endif // Include Guard: TANK_AI_CONTROLLER_H
