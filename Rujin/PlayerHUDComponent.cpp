#include "RujinPCH.h"
#include "PlayerHUDComponent.h"

#include "PeterPepperComponent.h"
#include "TextComponent.h"

#include "GameEvents.h"

using namespace rujin;

PlayerHUDComponent::PlayerHUDComponent(TextComponent* pLivesDisplay, TextComponent* pPointsDisplay, PeterPepperComponent* pPeterPepperComp, const SDL_Color& color)
	: m_pPlayer(pPeterPepperComp->GameObject())
	, m_pLivesDisplay(pLivesDisplay)
	, m_pPointsDisplay(pPointsDisplay)
{
	m_pLivesDisplay->SetColor(color);
	m_pPointsDisplay->SetColor(color);

	UpdateLivesDisplay(pPeterPepperComp->GetLives());
	UpdatePointsDisplay(pPeterPepperComp->GetPoints());

	/* Subscribe to subjects */
	pPeterPepperComp->AddObserver(this); //TODO: unsubscribe on destroy
}


void PlayerHUDComponent::Start()
{
	Component::Start();
}

void PlayerHUDComponent::Update()
{
	Component::Update();
}

void PlayerHUDComponent::OnNotify(const event::Identifier identifier, const event::Data* const pEventData)
{
	switch (identifier)
	{
	case event::Identifier::PlayerDied:
		OnPlayerDied(static_cast<const event::PlayerDiedData* const>(pEventData));
		break;
	case event::Identifier::PlayerGainedPoints:
		OnPlayerGainedPoints(static_cast<const event::PlayerGainedPointsData* const>(pEventData));
		break;
	}

}

void PlayerHUDComponent::OnPlayerDied(const event::PlayerDiedData* pEventData)
{
	// if the player that died is the player we are displaying information for.
	if (pEventData->m_pPlayer == m_pPlayer)
	{
		UpdateLivesDisplay(pEventData->m_RemainingLives);
	}
}

void PlayerHUDComponent::OnPlayerGainedPoints(const event::PlayerGainedPointsData* pEventData)
{
	if (pEventData->m_pPlayer == m_pPlayer)
	{
		UpdatePointsDisplay(pEventData->m_NewPoints);
	}
}

void PlayerHUDComponent::UpdateLivesDisplay(const uint16_t lives)
{
	m_pLivesDisplay->SetText("Lives: " + std::to_string(lives));
}

void PlayerHUDComponent::UpdatePointsDisplay(const uint32_t points)
{
	m_pPointsDisplay->SetText("Points: " + std::to_string(points));
}
