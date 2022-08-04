#ifndef TRON_PLAYER_COMPONENT_H
#define TRON_PLAYER_COMPONENT_H
#include "Component.h"
#include "InputStructs.h"


class TronPlayerComponent : public rujin::Component
{
public:
	TronPlayerComponent();

	void FixedUpdate() override;

private:

	float m_MoveSpeed{200.f};
	PlayerIndex m_PlayerIdx;
};


#endif // Include Guard: TRON_PLAYER_COMPONENT_H
