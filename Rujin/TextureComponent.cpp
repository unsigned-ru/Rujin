#include "RujinPCH.h"
#include "TextureComponent.h"


rujin::TextureComponent::TextureComponent(const std::weak_ptr<GameObject> gameObject)
	: Component(gameObject)
{
}


void rujin::TextureComponent::SetTexture(const std::shared_ptr<Texture2D> texture)
{
	m_Texture = texture;
}

std::weak_ptr<rujin::Texture2D> rujin::TextureComponent::GetTexture() const
{
	return m_Texture;
}
