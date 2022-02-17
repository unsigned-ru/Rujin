#include "RujinPCH.h"
#include "FpsComponent.h"

#include "Rujin.h"
#include "TextComponent.h"
#include "TextureRenderComponent.h"

rujin::FpsComponent::FpsComponent(const std::weak_ptr<GameObject>& gameObject)
	: Component(gameObject)
{
}

void rujin::FpsComponent::Update()
{
	Component::Update();

	//calculate FPS
	m_FpsTimer += Rujin::GetDeltaTime();
	++m_FpsCount;
	if (m_FpsTimer >= m_UpdateInterval)
	{
		m_Fps = m_FpsCount;
		m_FpsCount = 0;
		m_FpsTimer -= m_UpdateInterval;
	}

	//Update text
	m_TextComponent.lock()->SetText(std::to_string(m_Fps) + " FPS");
	m_TextureRenderComponent.lock()->SetTexture(m_TextComponent.lock()->GetTexture());

}

void rujin::FpsComponent::SetTextComponent(const std::weak_ptr<TextComponent> textComp)
{
	m_TextComponent = textComp;
}

void rujin::FpsComponent::SetTextureRenderComponent(const std::weak_ptr<TextureRenderComponent> textureRenderComp)
{
	m_TextureRenderComponent = textureRenderComp;
}
