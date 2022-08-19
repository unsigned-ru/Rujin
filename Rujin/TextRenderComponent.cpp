#include "RujinPCH.h"

#include "TextRenderComponent.h"

#include <SDL_ttf.h>
#include <cassert>

#include "FpsComponent.h"
#include "RenderService.h"
#include "ResourceService.h"
#include "ServiceLocator.h"

namespace rujin
{
	class ResourceService;
}

rujin::TextRenderComponent::TextRenderComponent(const std::shared_ptr<Font>& pFont, const glm::vec4& color)
	: m_Color(color)
	, m_pFont(pFont)
{
}

void rujin::TextRenderComponent::Start()
{
	assert(m_pFont);

	TextureRenderComponent::Start();
}

void rujin::TextRenderComponent::Update()
{
	if (m_IsDirty)
		UpdateTexture();

	TextureRenderComponent::Update();
}

void rujin::TextRenderComponent::Draw() const
{
	const auto& rs = ServiceLocator::GetService<RenderService>();
	rs.SetColor(m_Color);
	TextureRenderComponent::Draw();
	rs.SetColor();
}

// This implementation uses the "dirty flag" pattern
void rujin::TextRenderComponent::SetText(const std::string& text)
{
	m_Text = text;
	m_IsDirty = true;
}

void rujin::TextRenderComponent::SetFont(const std::shared_ptr<rujin::Font>& font)
{
	m_pFont = font;
	m_IsDirty = true;
}

void rujin::TextRenderComponent::SetColor(const glm::vec4& color)
{
	m_Color = color;
}

void rujin::TextRenderComponent::UpdateTexture()
{
	ServiceLocator::GetService<ResourceService>().GetStringTexture(m_Text, m_pFont);
}

bool rujin::TextRenderComponent::IsDirty() const
{
	return m_IsDirty;
}

