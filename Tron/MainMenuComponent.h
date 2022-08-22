#ifndef MAIN_MENU_COMPONENT_H
#define MAIN_MENU_COMPONENT_H
#include "Component.h"
#include "Tron.h"
#include "TronPlayerComponent.h"

namespace rujin
{
	class TextRenderComponent;
}

class MainMenuComponent : public Component
{
public:
	explicit MainMenuComponent(TextRenderComponent* pSinglePlayerText, TextRenderComponent* pCoOpText, TextRenderComponent* pPlayerVsPlayer);

	void Update() override;
private:

	std::array<TextRenderComponent*, 3> m_TextComponents;
	Tron::GameMode m_SelectedGamemode{ Tron::GameMode::SINGLE_PLAYER };
};


#endif // Include Guard: MAIN_MENU_COMPONENT_H
