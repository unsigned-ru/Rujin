#include "TronPCH.h"
#include "TronPlayerComponent.h"

#include "GameObject.h"
#include "InputEvents.h"
#include "InputService.h"
#include "ServiceLocator.h"
#include "TankAimingComponent.h"
#include "TankComponent.h"
#include "TankMovementComponent.h"
#include "TextureRenderComponent.h"


TronPlayerComponent::TronPlayerComponent(TankComponent* pTank)
	: m_PlayerIdx(ServiceLocator::GetService<InputService>().RegisterPlayer())
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
	ASSERT(m_pTank, "This component requires a TankComponent.");
}

void TronPlayerComponent::FixedUpdate()
{
	const InputService& input = ServiceLocator::GetService<InputService>();

	HandleMovement(input);

	HandleAiming(input);
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
			m_pTank->GetMovement()->MoveRight(inputIntensity, deltaTime);
		else
			//we want to move left
			m_pTank->GetMovement()->MoveLeft(inputIntensity, deltaTime);
	}
	else if (input.IsAxisActionTriggered(0, static_cast<uint32_t>(AxisAction::MoveUp), &inputIntensity))
	{
		if (inputIntensity > 0)
			//we want to move up
			m_pTank->GetMovement()->MoveUp(inputIntensity, deltaTime);
		else
			//we want to move down
			m_pTank->GetMovement()->MoveDown(inputIntensity, deltaTime);
	}
}

void TronPlayerComponent::HandleAiming(const InputService& input)
{
	bool inputTriggered = false;
	float inputIntensity;

	glm::vec2 aimingDirection{};

	if (input.IsAxisActionTriggered(0, static_cast<uint32_t>(AxisAction::AimUp), &inputIntensity))
	{
		inputTriggered = true;
		aimingDirection.y = inputIntensity;
	}

	if (input.IsAxisActionTriggered(0, static_cast<uint32_t>(AxisAction::AimRight), &inputIntensity))
	{
		inputTriggered = true;
		aimingDirection.x = inputIntensity;
	}

	if (inputTriggered)
		m_pTank->GetAiming()->AimAt(aimingDirection);
}

