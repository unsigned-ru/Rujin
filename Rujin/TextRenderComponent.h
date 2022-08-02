#ifndef TEXT_RENDER_COMPONENT_H
#define TEXT_RENDER_COMPONENT_H
#include "Component.h"

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
		TextComponent* m_pTextComponent = nullptr;
		TextureRenderComponent* m_pTextureRenderComponent = nullptr;
	};
}

#endif