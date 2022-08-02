#ifndef FPS_COMPONENT_H
#define FPS_COMPONENT_H
#include "Component.h"

namespace rujin
{
	class TextComponent;


	class FpsComponent final : public Component
	{
	public:
		explicit FpsComponent(TextComponent* pTextComponent);

		void Start() override;
		void Update() override;


	private:
		TextComponent* m_pTextComponent;

		float m_UpdateInterval{1.f};
		float m_FpsTimer{0.f};
		uint32_t m_FpsCount{0};

		uint32_t m_Fps{};
	};
}

#endif