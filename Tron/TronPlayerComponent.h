#ifndef TRON_PLAYER_COMPONENT_H
#define TRON_PLAYER_COMPONENT_H
#include "Component.h"
#include "InputStructs.h"


namespace rujin
{
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

private:
	void HandleMovement(const InputService& input, CollisionQuadTree* pCollisionTree);

	BoxColliderComponent* m_pBoxCollider = nullptr;

	float m_MoveSpeed{ 200.f };

	inline static float s_WallCheckOffset{ 1.f };
	inline static float s_WallCheckLengthReduction{ 4.f };

	PlayerIndex m_PlayerIdx;

	Direction m_FacingDirection{ Direction::UP };
};


#endif // Include Guard: TRON_PLAYER_COMPONENT_H
