#include "RujinPCH.h"
#include "TextureRenderComponent.h"
#include "GameObject.h"
#include "Renderer.h"
#include "TransformComponent.h"


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
