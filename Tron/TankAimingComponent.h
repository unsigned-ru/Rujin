#ifndef TANK_AIMING_COMPONENT_H
#define TANK_AIMING_COMPONENT_H
#include "Component.h"


class TankComponent;

class TankAimingComponent : public Component
{
public:
	explicit TankAimingComponent() = default;

	void AimAt(const glm::vec2& direction);
	void GetBulletSocket(Position& pos, Rotation& rot, glm::vec2& barrelDirection) const;

private:
	inline static constexpr float s_BulletSpawnDistance{ 50.f };
};


#endif // Include Guard: TANK_AIMING_COMPONENT_H
