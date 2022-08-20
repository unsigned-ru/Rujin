#ifndef RANDOM_TELEPORTER_COMPONENT_H
#define RANDOM_TELEPORTER_COMPONENT_H
#include "Component.h"
#include "SpawnLocationListComponent.h"

class RandomTeleporterComponent : public Component
{
public:
	explicit RandomTeleporterComponent() = default;

	void LateStart() override;

private:
	void OnOverlap(const CollisionResult& result) override;

	class GameObject* m_pManagerObject = nullptr;

};


#endif // Include Guard: RANDOM_TELEPORTER_COMPONENT_H
