#ifndef TRON_PLAYER_COMPONENT_H
#define TRON_PLAYER_COMPONENT_H
#include "Component.h"
#include "InputStructs.h"
#include "TankMovementComponent.h"


namespace rujin
{
	class TextureRenderComponent;
	class CollisionQuadTree;
	class InputService;
	class BoxColliderComponent;
}

class TronPlayerComponent : public rujin::Component
{
public:
	TronPlayerComponent();

	void Start() override;
	void FixedUpdate() override;
	void Draw() const override;

	PlayerIndex GetPlayerIndex() const;

private:
	void HandleMovement(const InputService& input);

	const PlayerIndex m_PlayerIdx;

	TankMovementComponent* m_pTankMovement = nullptr;
};


#endif // Include Guard: TRON_PLAYER_COMPONENT_H
