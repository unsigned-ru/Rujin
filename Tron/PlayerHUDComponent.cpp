#include "TronPCH.h"
#include "PlayerHUDComponent.h"

#include "GameEvents.h"
#include "GridTextureRenderComponent.h"
#include "TextRenderComponent.h"
#include "TronPlayerComponent.h"

PlayerHUDComponent::PlayerHUDComponent(TronPlayerComponent* pPlayer, GridTextureRenderComponent* pLivesRenderer, TextRenderComponent* pScoreRenderer)
	: m_pPlayer(pPlayer)
	, m_pLivesRenderer(pLivesRenderer)
	, m_pScoreRenderer(pScoreRenderer)
{
}

void PlayerHUDComponent::LateStart()
{
	Component::LateStart();
	m_pPlayer->AddObserver(this);
}

void PlayerHUDComponent::OnNotify(const uint32_t identifier, const event::Data* pEventData)
{
	if (identifier == static_cast<uint32_t>(game_event::Identifier::OnLivesChanged))
	{
		const auto* livesChangedEvent = static_cast<const game_event::OnLivesChanged_t*>(pEventData);

		m_pLivesRenderer->SetGridDimensions({ livesChangedEvent->newLives, 1 });
	}
	else if (identifier == static_cast<uint32_t>(game_event::Identifier::OnScoreChanged))
	{
		const auto* scoreChangedEvent = static_cast<const game_event::OnScoreChanged_t*>(pEventData);

		m_pScoreRenderer->SetText(std::to_string(scoreChangedEvent->newScore));
	}
}
