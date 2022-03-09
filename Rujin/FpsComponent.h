#pragma once
#include "Component.h"

namespace rujin
{
	class TextureRenderComponent;
	class TextComponent;

	class FpsComponent final : public Component
	{
	public:
		explicit FpsComponent(TextComponent* textComp, TextureRenderComponent* textRenderComp);

		void Start() override;
		void Update() override;

		void SetTextComponent(TextComponent* textComp);
		void SetTextureRenderComponent(TextureRenderComponent* textureRenderComp);

	private:
		TextComponent* m_pTextComponent{};
		TextureRenderComponent* m_pTextureRenderComponent{};

		float m_UpdateInterval{1.f};
		float m_FpsTimer{0.f};
		uint32_t m_FpsCount{0};

		uint32_t m_Fps{};

	};
}

