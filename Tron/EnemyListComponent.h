#ifndef ENEMY_LIST_COMPONENT_H
#define ENEMY_LIST_COMPONENT_H

#include "Component.h"
#include "IObserver.h"

class EnemyListComponent : public Component, public event::IObserver
{
public:
	EnemyListComponent() = default;
	void LateStart() override;

private:
	void OnNotify(const uint32_t identifier, const event::Data* pEventData) override;
	std::vector<class GameObject*> m_pEnemies;
};


#endif // Include Guard: ENEMY_LIST_COMPONENT_H
