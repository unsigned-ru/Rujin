#include "RujinPCH.h"

#include "GameObject.h"
#include "MainRenderProvider.h"

#include "TextureRenderComponent.h"

#include "ServiceLocator.h"
#include "Texture.h"

rujin::TextureRenderComponent::TextureRenderComponent(const std::shared_ptr<Texture>& texture, const glm::vec2& pivot, const Recti& sourceRect)
	: m_pTexture(texture)
	, m_Pivot(pivot)
	, m_SourceRect(sourceRect)
{}

void rujin::TextureRenderComponent::LateStart()
{
	assert(m_pTexture);
	SetSourceRect(m_SourceRect); //to ensure correct.
}

void rujin::TextureRenderComponent::Draw() const
{
	ServiceLocator::GetService<RenderService>().RenderTexture(*m_pTexture, GameObject()->GetTransform(), m_Pivot, &m_SourceRect, m_IsFlippedX, m_IsFlippedY);
}

void rujin::TextureRenderComponent::SetTexture(const std::shared_ptr<Texture>& texture)
{
 	m_pTexture = texture;
}

void rujin::TextureRenderComponent::SetSourceRect(const Recti& sourceRect)
{
	if (sourceRect == Recti{}) //if we passed default sourceRect, then we want to draw the entire texture.
		m_SourceRect = Recti{ 0, 0, m_pTexture->GetSize().x, m_pTexture->GetSize().y };
	else
		m_SourceRect = sourceRect;
}

void rujin::TextureRenderComponent::SetPivot(const glm::vec2& pivot)
{
	m_Pivot = pivot;
}

void rujin::TextureRenderComponent::SetFlippedX(const bool flipped)
{
	m_IsFlippedX = flipped;
}

void rujin::TextureRenderComponent::SetFlippedY(const bool flipped)
{
	m_IsFlippedY = flipped;
}

bool rujin::TextureRenderComponent::IsFlippedX() const
{
	return m_IsFlippedX;
}

bool rujin::TextureRenderComponent::IsFlippedY() const
{
	return m_IsFlippedY;
}
