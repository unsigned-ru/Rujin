#ifndef FPS_COMPONENT_H
#define FPS_COMPONENT_H
#include "Component.h"

namespace rujin
{
	class TextRenderComponent;


	class FpsComponent final : public Component
	{
	public:
		explicit FpsComponent(TextRenderComponent* pTextRenderer);

		void Start() override;
		void Update() override;


	private:
		TextRenderComponent* m_pTextRenderer;

		float m_UpdateInterval{1.f};
		float m_FpsTimer{0.f};
		uint32_t m_FpsCount{0};

		uint32_t m_Fps{};
	};
}

#endif