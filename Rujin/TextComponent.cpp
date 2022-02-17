#include "RujinPCH.h"
#include <SDL_ttf.h>
#include "TextComponent.h"
#include "Renderer.h"
#include "Font.h"
#include "GameObject.h"
#include "RectTransformComponent.h"
#include "Texture2D.h"

rujin::TextComponent::TextComponent(const std::weak_ptr<GameObject> gameObject)
	: Component(gameObject)
{

}


void rujin::TextComponent::Update()
{
	if (m_NeedsUpdate)
		GenerateTextTexture();
}


// This implementation uses the "dirty flag" pattern
void rujin::TextComponent::SetText(const std::string& text)
{
	m_Text = text;
	m_NeedsUpdate = true;
}

void rujin::TextComponent::SetFont(const std::weak_ptr<Font> font)
{
	m_Font = font;
	m_NeedsUpdate = true;
}

void rujin::TextComponent::SetPosition(const float x, const float y)
{
	m_GameObject.lock()->GetComponent<RectTransformComponent>().lock()->SetPosition({ x, y});
}

void rujin::TextComponent::SetColor(const SDL_Color color)
{
	m_Color = color;
}

void rujin::TextComponent::GenerateTextTexture()
{
	// TODO: warning
		if (m_Font.expired())
			return;

	const auto surf = TTF_RenderText_Blended(m_Font.lock()->GetFont(), m_Text.c_str(), m_Color);
	if (surf == nullptr)
		throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());

	auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
	if (texture == nullptr)
		throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());

	SDL_FreeSurface(surf);
	m_TextTexture = std::make_shared<Texture2D>(texture);
	m_NeedsUpdate = false;
}

std::weak_ptr<rujin::Texture2D> rujin::TextComponent::GetTexture() const
{
	return m_TextTexture;
}

