#include "RujinPCH.h"
#include "Components/TextRenderComponent.h"

#include "TextComponent.h"
#include "TextureRenderComponent.h"
#include "GameLoop/GameObject.h"

using namespace rujin;

TextRenderComponent::TextRenderComponent(TextComponent* const pTextComponent, TextureRenderComponent* const pTextureRenderComponent)
	: m_pTextComponent(pTextComponent)
	, m_pTextureRenderComponent(pTextureRenderComponent)
{
}

void TextRenderComponent::Start()
{
	Component::Start();

	if (!m_pTextComponent)
		m_pTextComponent = GameObject()->GetComponent<TextComponent>();

	if (!m_pTextureRenderComponent)
		m_pTextureRenderComponent = GameObject()->GetComponent<TextureRenderComponent>();

	assert(m_pTextComponent && m_pTextureRenderComponent);

	//setup an initial texture
	m_pTextureRenderComponent->SetTexture(m_pTextComponent->GenerateTexture());
}

void TextRenderComponent::Update()
{
	Component::Update();

	if (m_pTextComponent->IsDirty())
		m_pTextureRenderComponent->SetTexture(m_pTextComponent->GenerateTexture());
}

TextComponent* TextRenderComponent::GetText() const
{
	return m_pTextComponent;
}
