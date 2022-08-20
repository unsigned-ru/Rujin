#include "TronPCH.h"
#include "EnemyListComponent.h"

#include "EngineEvents.h"
#include "GameObject.h"
#include "Scene.h"
#include "SceneService.h"
#include "ServiceLocator.h"

void EnemyListComponent::LateStart()
{
	Component::LateStart();

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
		LOG_DEBUG("Loading next level!");
		ServiceLocator::GetService<SceneService>().SetActiveScene(ServiceLocator::GetService<SceneService>().GetScene("Level2"));
	}
}
