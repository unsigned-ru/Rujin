#include "RujinPCH.h"
#include "FpsComponent.h"

#include "Rujin.h"
#include "TextComponent.h"
#include "TextureRenderComponent.h"
#include <cassert>


rujin::FpsComponent::FpsComponent(TextComponent* textComp, TextureRenderComponent* textRenderComp)
	: m_pTextComponent(textComp)
	, m_pTextureRenderComponent(textRenderComp)
{
}

void rujin::FpsComponent::Start()
{
	Component::Start();

	assert(m_pTextComponent);
	assert(m_pTextureRenderComponent);

	//assign initial fps value
	m_pTextComponent->SetText("00 FPS");
	m_pTextureRenderComponent->SetTexture(m_pTextComponent->GenerateTexture());
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

		//update the text component only when we need to
		m_pTextComponent->SetText(std::to_string(m_Fps) + " FPS");
		m_pTextureRenderComponent->SetTexture(m_pTextComponent->GenerateTexture());
	}
}

void rujin::FpsComponent::SetTextComponent(TextComponent* textComp)
{
	m_pTextComponent = textComp;
}

void rujin::FpsComponent::SetTextureRenderComponent(TextureRenderComponent* textureRenderComp)
{
	m_pTextureRenderComponent = textureRenderComp;
}
