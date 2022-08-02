#include "TronPCH.h"
#include "TronPlayerComponent.h"

#include "GameObject.h"
#include "InputEvents.h"
#include "InputService.h"
#include "ServiceLocator.h"


TronPlayerComponent::TronPlayerComponent()
{
	InputService& input = ServiceLocator::GetService<InputService>();
	m_PlayerIdx = input.RegisterPlayer();

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

void TronPlayerComponent::Update()
{
	const InputService& input = ServiceLocator::GetService<InputService>();

	float inputIntensity;
	const float deltaTime = Rujin::Get()->GetDeltaTime();
	
	TransformComponent* pTransform = m_pGameObject->GetTransform();
	if (input.IsAxisActionTriggered(0, static_cast<uint32_t>(AxisAction::MoveRight), &inputIntensity))
		pTransform->AddLocalPosition({ inputIntensity * m_MoveSpeed * deltaTime, 0 });
	else if (input.IsAxisActionTriggered(0, static_cast<uint32_t>(AxisAction::MoveUp), &inputIntensity))
		pTransform->AddLocalPosition({ 0,  inputIntensity * m_MoveSpeed * deltaTime });
}

void TronPlayerComponent::OnOverlap(const CollisionResult& /*collision*/)
{
	LOG_DEBUG("Overlapping!");
}
