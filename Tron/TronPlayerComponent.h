#ifndef TRON_PLAYER_COMPONENT_H
#define TRON_PLAYER_COMPONENT_H
#include "Component.h"
#include "InputStructs.h"


class TankComponent;

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
	explicit TronPlayerComponent(TankComponent* pTank);

	void Start() override;
	void FixedUpdate() override;
	void Draw() const override;

	PlayerIndex GetPlayerIndex() const;

private:
	void HandleMovement(const InputService& input);
	void HandleAiming(const InputService& input);

	const PlayerIndex m_PlayerIdx;

	TankComponent* m_pTank = nullptr;
};


#endif // Include Guard: TRON_PLAYER_COMPONENT_H
