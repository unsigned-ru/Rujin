#include "RujinPCH.h"

#include "GameObject.h"
#include "Renderer.h"

#include "TextureRenderComponent.h"
#include "TransformComponent.h"

rujin::TextureRenderComponent::TextureRenderComponent(const std::shared_ptr<Texture>& texture, const glm::vec2& pivot)
	: m_pTexture(texture)
	, m_Pivot(pivot)
{
}

void rujin::TextureRenderComponent::LateStart()
{
	assert(m_pTexture);
}

void rujin::TextureRenderComponent::Draw() const
{
	const auto transform = GameObject()->GetTransform();

	Renderer::Get()->RenderTexture(*m_pTexture, transform->GetTransform(), m_Pivot);
}

void rujin::TextureRenderComponent::SetTexture(const std::shared_ptr<Texture>& texture)
{
	m_pTexture = texture;
}

void rujin::TextureRenderComponent::SetPivot(const glm::vec2& pivot)
{
	m_Pivot = pivot;
}
