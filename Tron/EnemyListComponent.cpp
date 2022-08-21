#include "TronPCH.h"
#include "EnemyListComponent.h"

#include "EngineEvents.h"
#include "GameObject.h"
#include "Scene.h"
#include "Tron.h"

void EnemyListComponent::Update()
{
	Component::Update();

	if (m_IsDirty && ++m_DirtyFrameCounter > 1)
	{
		m_IsDirty = false;
		m_DirtyFrameCounter = 0;
		UpdateEnemyList();
	}
}

void EnemyListComponent::SetDirty(bool dirty)
{
	m_IsDirty = dirty;
}

void EnemyListComponent::UpdateEnemyList()
{
	m_pEnemies = GameObject()->GetScene()->GetAllRootGameObjectsByPredicate([](const rujin::GameObject* pGameObject) { return  pGameObject->HasTag("Enemy"); });

	for (class GameObject* pEnemyGO : m_pEnemies)
	{
		pEnemyGO->AddObserver(this);
	}
}

void EnemyListComponent::OnNotify(const uint32_t identifier, const event::Data* pEventData)
{
 	if (identifier == static_cast<uint32_t>(event::Identifier::OnGameObjectDestroyed))
	{
		const auto* pGameObjectDestroyedEvent = static_cast<const event::OnGameObjectDestroyed_t*>(pEventData);

		m_pEnemies.erase(std::find(m_pEnemies.begin(), m_pEnemies.end(), pGameObjectDestroyedEvent->pObject));
	}

	if (m_pEnemies.size() == 0)
	{
		//all enemies died, we want to load next level!
 		Rujin::Get()->GetGame<Tron>()->SwitchToNextLevel();
	}
}
