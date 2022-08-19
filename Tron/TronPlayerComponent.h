#ifndef TRON_PLAYER_COMPONENT_H
#define TRON_PLAYER_COMPONENT_H
#include "Component.h"
#include "InputStructs.h"
#include "IObserver.h"


class TankComponent;

namespace rujin
{
	class InputService;
}

class TronPlayerComponent : public Component, public event::IObserver
{
public:
	explicit TronPlayerComponent(TankComponent* pTank, PlayerIndex playerIndex);

	void Start() override;
	void FixedUpdate() override;
	void Draw() const override;

	PlayerIndex GetPlayerIndex() const;

private:
	void HandleMovement(const InputService& input);
	void HandleAiming(const InputService& input);

private:
	void OnNotify(const uint32_t identifier, const event::Data* pEventData) override;

	const PlayerIndex m_PlayerIdx;

	TankComponent* m_pTank = nullptr;
};


#endif // Include Guard: TRON_PLAYER_COMPONENT_H
