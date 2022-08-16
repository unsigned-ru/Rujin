#ifndef TANK_AIMING_COMPONENT_H
#define TANK_AIMING_COMPONENT_H
#include "Component.h"


class TankComponent;

class TankAimingComponent : public Component
{
public:
	explicit TankAimingComponent(float bulletSpawnDistance = 50.f);

	void AimAt(const glm::vec2& direction);
	void GetBulletSocket(Position& pos, Rotation& rot, glm::vec2& barrelDirection) const;

private:
	const float m_BulletSpawnDistance{ 50.f };
};


#endif // Include Guard: TANK_AIMING_COMPONENT_H
