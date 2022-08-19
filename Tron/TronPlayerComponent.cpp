#include "TronPCH.h"
#include "TronPlayerComponent.h"

#include "GameObject.h"
#include "HealthComponent.h"
#include "InputEvents.h"
#include "InputService.h"
#include "ServiceLocator.h"
#include "TankAimingComponent.h"
#include "TankComponent.h"
#include "TronMovementComponent.h"
#include "GameEvents.h"


TronPlayerComponent::TronPlayerComponent(TankComponent* pTank, PlayerIndex playerIndex)
	: m_PlayerIdx(playerIndex)
	, m_pTank(pTank)
{
	/* Register player*/
	InputService& input = ServiceLocator::GetService<InputService>();

	/* Register actions */
	//Axis actions
	input.AddAxisAction
	(
		m_PlayerIdx,
		static_cast<uint32_t>(AxisAction::MoveRight),
		AxisActionKeybinds{ KeyboardAxisKeybinds{'D', 'A'}, GamepadAxisKeybinds{GamepadButton::DPAD_RIGHT, GamepadButton::DPAD_LEFT} }
	);

	input.AddAxisAction
	(
		m_PlayerIdx,
		static_cast<uint32_t>(AxisAction::MoveUp),
		AxisActionKeybinds{ KeyboardAxisKeybinds{'W', 'S'}, GamepadAxisKeybinds{GamepadButton::DPAD_UP, GamepadButton::DPAD_DOWN} }
	);

	input.AddAxisAction
	(
		m_PlayerIdx,
		static_cast<uint32_t>(AxisAction::AimUp),
		AxisActionKeybinds{ KeyboardAxisKeybinds{VK_UP, VK_DOWN}, GamepadAxisKeybinds{ GamepadButton::RIGHT_STICK_Y } }
	);

	input.AddAxisAction
	(
		m_PlayerIdx,
		static_cast<uint32_t>(AxisAction::AimRight),
		AxisActionKeybinds{ KeyboardAxisKeybinds{VK_RIGHT, VK_LEFT}, GamepadAxisKeybinds{ GamepadButton::RIGHT_STICK_X } }
	);

	//Input Actions
	input.AddInputAction
	(
		m_PlayerIdx,
		static_cast<uint32_t>(InputAction::Shoot),
		InputActionKeybinds{ ButtonState::Released, VK_SPACE, GamepadButton::RIGHT_TRIGGER }
	);

}

void TronPlayerComponent::Start()
{
	ASSERT_MSG(m_pTank, "This component requires a TankComponent.");

	m_pTank->GetHealthComponent()->AddObserver(this);
}

void TronPlayerComponent::FixedUpdate()
{
	const InputService& input = ServiceLocator::GetService<InputService>();

	HandleMovement(input);

	HandleAiming(input);

	if (input.IsInputActionTriggered(m_PlayerIdx, static_cast<uint32_t>(InputAction::Shoot)))
		m_pTank->Shoot();
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
	const float deltaTime = Rujin::Get()->GetFixedUpdateDeltaTime();

	if (input.IsAxisActionTriggered(m_PlayerIdx, static_cast<uint32_t>(AxisAction::MoveRight), &inputIntensity))
	{
		if (inputIntensity > 0)
			//we want to move right
			m_pTank->GetMovement()->MoveRight(abs(inputIntensity), deltaTime);
		else
			//we want to move left
			m_pTank->GetMovement()->MoveLeft(abs(inputIntensity), deltaTime);
	}
	else if (input.IsAxisActionTriggered(m_PlayerIdx, static_cast<uint32_t>(AxisAction::MoveUp), &inputIntensity))
	{
		if (inputIntensity > 0)
			//we want to move up
			m_pTank->GetMovement()->MoveUp(abs(inputIntensity), deltaTime);
		else
			//we want to move down
			m_pTank->GetMovement()->MoveDown(abs(inputIntensity), deltaTime);
	}
}

void TronPlayerComponent::HandleAiming(const InputService& input)
{
	bool inputTriggered = false;
	float inputIntensity;

	glm::vec2 aimingDirection{};

	if (input.IsAxisActionTriggered(m_PlayerIdx, static_cast<uint32_t>(AxisAction::AimUp), &inputIntensity))
	{
		inputTriggered = true;
		aimingDirection.y = inputIntensity;
	}

	if (input.IsAxisActionTriggered(m_PlayerIdx, static_cast<uint32_t>(AxisAction::AimRight), &inputIntensity))
	{
		inputTriggered = true;
		aimingDirection.x = inputIntensity;
	}

	if (inputTriggered)
	{
		LOG_DEBUG_("Aiming direction: [{}, {}]", aimingDirection.x, aimingDirection.y);
		m_pTank->GetAiming()->AimAt(aimingDirection);
	}
}

void TronPlayerComponent::OnNotify(const uint32_t identifier, const event::Data* pEventData)
{
	if (identifier == static_cast<uint32_t>(game_event::Identifier::OnDie))
	{
		const auto* onDieData = static_cast<const game_event::OnDie_t*>(pEventData);

		//make sure it is us that has died...
		if (onDieData->pHealth != m_pTank->GetHealthComponent())
			return;

		GameObject()->Destroy();
	}
}

