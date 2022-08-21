#ifndef ENEMY_LIST_COMPONENT_H
#define ENEMY_LIST_COMPONENT_H

#include "Component.h"
#include "IObserver.h"

class EnemyListComponent : public Component, public event::IObserver
{
public:
	EnemyListComponent() = default;

	void Update() override;

	void SetDirty(bool dirty = true);

private:
	void UpdateEnemyList();
	void OnNotify(const uint32_t identifier, const event::Data* pEventData) override;
	std::vector<class GameObject*> m_pEnemies;

	bool m_IsDirty{ true };
	uint8_t m_DirtyFrameCounter{0};
};


#endif // Include Guard: ENEMY_LIST_COMPONENT_H
