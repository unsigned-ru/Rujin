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
#include "Scene.h"
#include "SceneService.h"
#include "SpawnLocationListComponent.h"



TronPlayerComponent::TronPlayerComponent(TankComponent* pTank, PlayerIndex playerIndex)
	: Subject({})
	, m_PlayerIdx(playerIndex)
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
	Component::Start();

	m_pManagerObject = GameObject()->GetScene()->GetRootGameObjectByPredicate([](const class GameObject* pObj) {return pObj->GetName() == "Manager"; });
	ASSERT(m_pManagerObject)

	GameObject()->AddTag("Player");
	GameObject()->AddTag("LevelPersistent");
	m_pTank->GetHealthComponent()->AddObserver(this);
}

void TronPlayerComponent::FixedUpdate()
{
	if (!m_pTank)
		return;

	const InputService& input = ServiceLocator::GetService<InputService>();

	HandleMovement(input);

	HandleAiming(input);

	if (input.IsInputActionTriggered(m_PlayerIdx, static_cast<uint32_t>(InputAction::Shoot)))
		m_pTank->Shoot();
}

PlayerIndex TronPlayerComponent::GetPlayerIndex() const
{
	return m_PlayerIdx;
}

uint8_t TronPlayerComponent::GetLives() const { return m_Lives; }

uint32_t TronPlayerComponent::GetScore() const { return m_Score; }

void TronPlayerComponent::AddScore(uint32_t scoreToAdd)
{
	if (scoreToAdd == 0) return;

	m_Score += scoreToAdd;

	const game_event::OnScoreChanged_t scoreChangedEvent(this, m_Score);
	Notify(static_cast<uint32_t>(game_event::Identifier::OnScoreChanged), &scoreChangedEvent);
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
		m_pTank->GetAiming()->AimAt(aimingDirection);
	}
}

void TronPlayerComponent::OnNotify(const uint32_t identifier, const event::Data* pEventData)
{
	if (identifier == static_cast<uint32_t>(game_event::Identifier::OnDie))
	{
		const auto* onDieData = static_cast<const game_event::OnDie_t*>(pEventData);

		//make sure it is us that has died...
		auto* pTankHealth = m_pTank->GetHealthComponent();
		if (onDieData->pHealth != pTankHealth)
			return;

		if (m_Lives-- > 0)
		{
			//set our health back to max
			pTankHealth->SetHealth(pTankHealth->GetMaxHealth());

			//randomize position1
			GameObject()->GetTransform().SetLocalPosition(m_pManagerObject->GetComponent<SpawnLocationListComponent>()->GetRandomSpawnLocation());

			//notify LivesChanged
			const game_event::OnLivesChanged_t livesChangedEvent{ this, m_Lives };
			Notify(static_cast<uint32_t>(game_event::Identifier::OnLivesChanged), &livesChangedEvent);
		}
		else
		{
			//... We fully died, game over.
		}
	}
}

