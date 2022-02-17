#pragma once
#include "Component.h"

namespace rujin
{
	class TextureRenderComponent;
	class TextComponent;

	class FpsComponent final : public Component
	{
	public:
		explicit FpsComponent(const std::weak_ptr<GameObject>& gameObject);

		void Update() override;

		void SetTextComponent(const std::weak_ptr<TextComponent> textComp);
		void SetTextureRenderComponent(const std::weak_ptr<TextureRenderComponent> textureRenderComp);


	private:
		std::weak_ptr<TextComponent> m_TextComponent{};
		std::weak_ptr<TextureRenderComponent> m_TextureRenderComponent{};

		float m_UpdateInterval{1.f};
		float m_FpsTimer{0.f};
		uint32_t m_FpsCount{0};

		uint32_t m_Fps{};

	};
}

