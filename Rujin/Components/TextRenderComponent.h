#pragma once
#include "Components/Component.h"

namespace rujin
{
	class TextureRenderComponent;
	class TextComponent;

	/**
	 * \brief Handles runtime updating of the texture renderer when text is changed.
	 */
	class TextRenderComponent final : public Component
	{
	public:
		explicit TextRenderComponent() = default;
		explicit TextRenderComponent(TextComponent* const pTextComponent, TextureRenderComponent* const pTextureRenderComponent);

		void Start() override;
		void Update() override;

		TextComponent* GetText() const;

	private:
		TextComponent* m_pTextComponent;
		TextureRenderComponent*  m_pTextureRenderComponent;
	};
}

