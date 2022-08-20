#ifndef PLAYER_HUD_COMPONENT_H
#define PLAYER_HUD_COMPONENT_H
#include "Component.h"
#include "IObserver.h"

class TronPlayerComponent;

namespace rujin
{
	class GridTextureRenderComponent;
	class TextRenderComponent;
}

class PlayerHUDComponent : public Component, public event::IObserver
{
public:
	explicit PlayerHUDComponent(TronPlayerComponent* pPlayer, GridTextureRenderComponent* pLivesRenderer, TextRenderComponent* pScoreRenderer);

	void Start() override;

private:
	void OnNotify(const uint32_t identifier, const event::Data* pEventData) override;

	TronPlayerComponent* m_pPlayer;
	GridTextureRenderComponent* m_pLivesRenderer;
	TextRenderComponent* m_pScoreRenderer;
};


#endif // Include Guard: PLAYER_HUD_COMPONENT_H
