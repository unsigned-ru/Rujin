#ifndef TEXT_RENDER_COMPONENT_H
#define TEXT_RENDER_COMPONENT_H
#include "Font.h"
#include "Texture.h"
#include "TextureRenderComponent.h"

namespace rujin
{

	class TextRenderComponent final : public TextureRenderComponent
	{
	public:
		explicit TextRenderComponent() = default;
		explicit TextRenderComponent(const std::shared_ptr<Font>& pFont, const glm::vec4& color = glm::vec4{1.f, 1.f, 1.f, 1.f}, const glm::vec2& pivot = { 0.f, 0.5f });

		void LateStart() override;
		void Update() override;
		void Draw() const override;

		void SetText(const std::string& text);
		void SetFont(const std::shared_ptr<Font>& font);
		void SetColor(const glm::vec4& color);

		bool IsDirty() const;

	private:
		void UpdateTexture();

		bool m_IsDirty{ true };
		std::string m_Text{"Sample Text"};
		glm::vec4 m_Color{1.f, 1.f, 1.f, 1.f};

		std::shared_ptr<Font> m_pFont;
	};
}
#endif