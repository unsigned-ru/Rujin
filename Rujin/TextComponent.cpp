#include "RujinPCH.h"

#include "TextComponent.h"
#include "Renderer.h"
#include "Font.h"

#include <SDL_ttf.h>
#include <cassert>

rujin::TextComponent::TextComponent(const std::shared_ptr<Font>& pFont, const SDL_Color& color)
	: m_pFont(pFont)
	, m_Color(color)
{

}

void rujin::TextComponent::Start()
{
	Component::Start();

	assert(m_pFont);
}

// This implementation uses the "dirty flag" pattern
void rujin::TextComponent::SetText(const std::string& text)
{
	m_Text = text;
	m_IsDirty = true;
}

void rujin::TextComponent::SetFont(const std::shared_ptr<Font>& font)
{
	m_pFont = font;
	m_IsDirty = true;
}

void rujin::TextComponent::SetColor(const SDL_Color color)
{
	m_Color = color;
}

std::shared_ptr<rujin::Texture> rujin::TextComponent::GenerateTexture()
{
	//assert(m_pFont);

	////if we don't need an update, and our weak pointer to the last generated texture still holds
	//if (!m_IsDirty && !m_pLastGeneratedTexture.expired())
	//	//return the last generated texture, no need to try and regenerate a new one
	//	return m_pLastGeneratedTexture.lock();

	//const auto surf = TTF_RenderText_Blended(m_pFont->GetFont(), m_Text.c_str(), m_Color);

	//if (!surf) 
	//	throw std::runtime_error(std::string("Draw text failed: ") + SDL_GetError());

	//const auto texture = SDL_CreateTextureFromSurface(Renderer::Get()->GetSDLRenderer(), surf);

	//if (!texture)
	//	throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());

	//SDL_FreeSurface(surf);

	//const auto rv = std::make_shared<Texture>(texture);

	////store a weak pointer to the texture, in case the exact same text texture gets requested again,
	////we can just share the resource instead of making a duplicate.
	//m_pLastGeneratedTexture = rv;

	//return rv;
	return nullptr;
}

bool rujin::TextComponent::IsDirty() const
{
	return m_IsDirty;
}

