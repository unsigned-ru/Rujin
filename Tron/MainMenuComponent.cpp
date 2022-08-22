#include "TronPCH.h"
#include "MainMenuComponent.h"

#include "InputEvents.h"
#include "InputService.h"
#include "ServiceLocator.h"
#include "TextRenderComponent.h"

namespace rujin
{
	class InputService;
}

MainMenuComponent::MainMenuComponent(TextRenderComponent* pSinglePlayerText, TextRenderComponent* pCoOpText, TextRenderComponent* pPlayerVsPlayer)
	: m_TextComponents
	({ pSinglePlayerText, pCoOpText, pPlayerVsPlayer })
{
	auto& input = ServiceLocator::GetService<InputService>();

	/* Register actions */
	//Input Actions
	input.AddInputAction
	(
		0,
		static_cast<uint32_t>(InputAction::MenuUp),
		InputActionKeybinds{ ButtonState::Released, 'W', GamepadButton::DPAD_UP}
	);

	input.AddInputAction
	(
		0,
		static_cast<uint32_t>(InputAction::MenuDown),
		InputActionKeybinds{ ButtonState::Released, 'S', GamepadButton::DPAD_DOWN }
	);

	input.AddInputAction
	(
		0,
		static_cast<uint32_t>(InputAction::Accept),
		InputActionKeybinds{ ButtonState::Released, VK_RETURN, GamepadButton::START }
	);

	m_TextComponents[static_cast<uint8_t>(m_SelectedGamemode)]->SetColor(rutils::colors::Aquamarine);
}

void MainMenuComponent::Update()
{
	Component::Update();

	const auto& input = ServiceLocator::GetService<InputService>();
	bool selectionChanged{ false };

	if (input.IsInputActionTriggered(0, static_cast<uint32_t>(InputAction::MenuUp)))
	{
		if (static_cast<int8_t>(m_SelectedGamemode) - 1 < 0)
		{
			m_SelectedGamemode = Tron::GameMode::PLAYER_VS_PLAYER;
		}
		else
		{
			m_SelectedGamemode = static_cast<Tron::GameMode>(static_cast<int8_t>(m_SelectedGamemode) - 1);
		}

		selectionChanged = true;
	}

	if (input.IsInputActionTriggered(0, static_cast<uint32_t>(InputAction::MenuDown)))
	{
		m_SelectedGamemode = static_cast<Tron::GameMode>((static_cast<uint8_t>(m_SelectedGamemode) + 1) % (static_cast<uint8_t>(Tron::GameMode::PLAYER_VS_PLAYER) + 1));

		selectionChanged = true;
	}

	if (selectionChanged)
	{
		for (TextRenderComponent* pComp : m_TextComponents)
			pComp->SetColor(rutils::colors::White);

		m_TextComponents[static_cast<uint8_t>(m_SelectedGamemode)]->SetColor(rutils::colors::Aquamarine);
	}


	if (input.IsInputActionTriggered(0, static_cast<uint32_t>(InputAction::Accept)))
	{
		//accept...
		Rujin::Get()->GetGame<Tron>()->StartGame(m_SelectedGamemode);
	}

}
