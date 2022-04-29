#include "RujinPCH.h"

#include "GameObject.h"
#include "Renderer.h"

#include "TextureRenderComponent.h"
#include "TransformComponent.h"

rujin::TextureRenderComponent::TextureRenderComponent(const std::shared_ptr<Texture2D>& texture)
	: m_pTexture(texture)
{
}

void rujin::TextureRenderComponent::LateStart()
{
	Component::LateStart();

	assert(m_pTexture);
}

void rujin::TextureRenderComponent::Render() const
{
	Component::Render();

	const auto transform = GameObject()->GetTransform();

	Renderer::GetInstance().RenderTexture(*m_pTexture, transform->GetPosition().x, transform->GetPosition().y);
}

void rujin::TextureRenderComponent::SetTexture(const std::shared_ptr<Texture2D>& texture)
{
	m_pTexture = texture;
}
