#ifndef TANK_MOVEMENT_COMPONENT_H
#define TANK_MOVEMENT_COMPONENT_H
#include "Component.h"

class TankComponent;
class TronPlayerComponent;

namespace rujin
{
	class BoxColliderComponent;
	class TextureRenderComponent;
}

class TankMovementComponent final : public Component
{
public:
	explicit TankMovementComponent() = default;

	void Start() override;

	void Move(Direction dir, float inputIntensity, float deltaTime);

	void MoveRight(float inputIntensity, float deltaTime);
	void MoveLeft(float inputIntensity, float deltaTime);
	void MoveUp(float inputIntensity, float deltaTime);
	void MoveDown(float inputIntensity, float deltaTime);

	void Move_Unchecked(Direction dir, float inputIntensity, float deltaTime);
	void MoveRight_Unchecked(float inputIntensity, float deltaTime);
	void MoveLeft_Unchecked(float inputIntensity, float deltaTime);
	void MoveUp_Unchecked(float inputIntensity, float deltaTime);
	void MoveDown_Unchecked(float inputIntensity, float deltaTime);

	void Draw() const override;

	void SetFacingDirection(Direction newDirection);

	bool CanMoveInDirection(Direction dir) const;

	bool CanMoveRight() const;
	bool CanMoveLeft() const;
	bool CanMoveUp() const;
	bool CanMoveDown() const;

	Direction GetFacingDirection() const;

private:
	friend class TankComponent;
	void SetTank(TankComponent* pTank);

	TankComponent* m_pTank = nullptr;

	Direction m_FacingDirection{ Direction::UP };

	inline static constexpr float m_MoveSpeed{ 200.f };
	inline static constexpr float s_WallCheckOffset{ 1.f };
	inline static constexpr float s_WallCheckLengthReduction{ 2.f };
};

#endif // Include Guard: TANK_MOVEMENT_COMPONENT_H
