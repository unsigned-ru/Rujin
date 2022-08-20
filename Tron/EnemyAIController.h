#ifndef ENEMY_AI_CONTROLLER_H
#define ENEMY_AI_CONTROLLER_H
#include "Component.h"
#include "IObserver.h"

class EnemyAIController : public Component
{
public:
	EnemyAIController() = default;

	void Start() override;

protected:
	bool IsInRange(rujin::GameObject* pObj, const float range) const;
	bool IsInLineOfSight(rujin::GameObject* pObj, const std::vector<const Collider*>& ignore) const;
	Direction GetDirectionToObject(rujin::GameObject* pObj) const;
};


#endif // Include Guard: ENEMY_AI_CONTROLLER_H
