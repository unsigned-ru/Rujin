#include "TronPCH.h"
#include "RandomTeleporterComponent.h"

#include "BoxColliderComponent.h"
#include "GameObject.h"
#include "Scene.h"
#include "TextureRenderComponent.h"

void RandomTeleporterComponent::LateStart()
{
	Component::LateStart();

	//we expect these components on the gameobject:
	ASSERT(GameObject()->GetComponent<BoxColliderComponent>());
	ASSERT(GameObject()->GetComponent<TextureRenderComponent>());

	m_pManagerObject = GameObject()->GetScene()->GetRootGameObjectByPredicate([](const class GameObject* pObj) {return pObj->GetName() == "Manager"; });
	ASSERT(m_pManagerObject)
}

void RandomTeleporterComponent::OnOverlap(const CollisionResult& result)
{
	auto* pOtherGO = result.other->GetComponent()->GameObject();

	if (pOtherGO->HasTag("Player") || pOtherGO->HasTag("Enemy"))
	{
		//randomize position
		pOtherGO->GetTransform().SetLocalPosition(m_pManagerObject->GetComponent<SpawnLocationListComponent>()->GetRandomSpawnLocation());
	}
}
