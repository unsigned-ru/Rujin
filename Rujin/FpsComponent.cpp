#include "RujinPCH.h"
#include "FpsComponent.h"

#include "Rujin.h"
#include "TextRenderComponent.h"
#include <cassert>


rujin::FpsComponent::FpsComponent(TextRenderComponent* pTextRenderer)
	: m_pTextRenderer(pTextRenderer)
{
}

void rujin::FpsComponent::Start()
{
	Component::Start();

	assert(m_pTextRenderer);

	//assign initial fps value
	m_pTextRenderer->SetText("00 FPS");
}

void rujin::FpsComponent::Update()
{
	Component::Update();

	//calculate FPS
	m_FpsTimer += Rujin::Get()->GetDeltaTime();
	++m_FpsCount;
	if (m_FpsTimer >= m_UpdateInterval)
	{
		m_Fps = m_FpsCount;
		m_FpsCount = 0;
		m_FpsTimer -= m_UpdateInterval;

		//update the text component only when we need to
		m_pTextRenderer->SetText(std::to_string(m_Fps) + " FPS");
	}
}
