#pragma once
#include "Component.h"
#include "IObserver.h"

namespace rujin
{
	class TextureRenderComponent;
	class PeterPepperComponent;
	class TextComponent;

	namespace event
	{
		enum class Identifier;
		struct Data;
		struct PlayerDiedData;
		struct PlayerGainedPointsData;
	}

	class PlayerHUDComponent final : public Component, public event::IObserver
	{
	public:
		explicit PlayerHUDComponent(TextComponent* const pLivesDisplay, TextComponent* const pPointsDisplay, PeterPepperComponent* const pPeterPepperComp, const SDL_Color& color);

		void Start() override;
		void Update() override;

	protected:
		void OnNotify(const event::Identifier identifier, const event::Data* const pEventData) override;

	private:
		rujin::GameObject* m_pPlayer;

		TextComponent* m_pLivesDisplay;
		TextComponent* m_pPointsDisplay;

		//Subscribed Events
		void OnPlayerDied(const event::PlayerDiedData* pEventData);
		void OnPlayerGainedPoints(const event::PlayerGainedPointsData* pEventData);

		//member functions
		void UpdateLivesDisplay(const uint16_t lives);
		void UpdatePointsDisplay(const uint32_t points);
	};
}

