#include "TronPCH.h"
#include "TronPlayerComponent.h"

#include "GameObject.h"
#include "InputEvents.h"
#include "InputService.h"
#include "ServiceLocator.h"
#include "TextureRenderComponent.h"


TronPlayerComponent::TronPlayerComponent()
	: m_PlayerIdx(ServiceLocator::GetService<InputService>().RegisterPlayer())
{
	/* Register player*/
	InputService& input = ServiceLocator::GetService<InputService>();

	/* Register actions */
	input.AddAxisAction
	(
		m_PlayerIdx,
		static_cast<uint32_t>(AxisAction::MoveRight),
		AxisActionKeybinds{ KeyboardAxisKeybinds{'D', 'A'} }
	);

	input.AddAxisAction
	(
		m_PlayerIdx,
		static_cast<uint32_t>(AxisAction::MoveUp),
		AxisActionKeybinds{ KeyboardAxisKeybinds{'W', 'S'} }
	);
}

void TronPlayerComponent::Start()
{
	m_pTankMovement = GameObject()->GetComponent<TankMovementComponent>();
	assert(m_pTankMovement); //This component requires a Tank movement component on the gameobject.
}

void TronPlayerComponent::FixedUpdate()
{
	const InputService& input = ServiceLocator::GetService<InputService>();

	HandleMovement(input);
}

void TronPlayerComponent::Draw() const
{
}

PlayerIndex TronPlayerComponent::GetPlayerIndex() const
{
	return m_PlayerIdx;
}

void TronPlayerComponent::HandleMovement(const InputService& input)
{
	float inputIntensity;
	const float deltaTime = Rujin::Get()->GetDeltaTime();

	if (input.IsAxisActionTriggered(0, static_cast<uint32_t>(AxisAction::MoveRight), &inputIntensity))
	{
		if (inputIntensity > 0)
			//we want to move right
			m_pTankMovement->MoveRight(inputIntensity, deltaTime);
		else
			//we want to move left
			m_pTankMovement->MoveLeft(inputIntensity, deltaTime);
	}
	else if (input.IsAxisActionTriggered(0, static_cast<uint32_t>(AxisAction::MoveUp), &inputIntensity))
	{
		if (inputIntensity > 0)
			//we want to move up
			m_pTankMovement->MoveUp(inputIntensity, deltaTime);
		else
			//we want to move down
			m_pTankMovement->MoveDown(inputIntensity, deltaTime);
	}
}
