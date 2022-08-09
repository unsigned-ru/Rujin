#ifndef TANK_AIMING_COMPONENT_H
#define TANK_AIMING_COMPONENT_H
#include "Component.h"


class TankAimingComponent : public Component
{
public:
	explicit TankAimingComponent() = default;

	void AimAt(const glm::vec2& direction);
private:
	inline static constexpr float s_BulletSpawnDistance{ 25.f };

	glm::vec2 GetSocketOffset() const;

	void Draw() const override;
};


#endif // Include Guard: TANK_AIMING_COMPONENT_H
