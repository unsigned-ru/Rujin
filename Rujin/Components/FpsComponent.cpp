#include "RujinPCH.h"
#include "Components/FpsComponent.h"

#include "Engine/Rujin.h"
#include "Components/TextComponent.h"
#include <cassert>


rujin::FpsComponent::FpsComponent(TextComponent* pTextComponent)
	: m_pTextComponent(pTextComponent)
{
}

void rujin::FpsComponent::Start()
{
	Component::Start();

	assert(m_pTextComponent);

	//assign initial fps value
	m_pTextComponent->SetText("00 FPS");
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
	}
}
