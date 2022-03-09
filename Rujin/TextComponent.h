#pragma once
#include "DynamicTexture2D.h"
#include "TransformComponent.h"

namespace rujin
{
	class Font;

	class TextComponent final : public Component
	{
	public:
		explicit TextComponent() = default;
		~TextComponent() override = default;
		TextComponent(const TextComponent & other) = delete;
		TextComponent(TextComponent && other) = delete;
		TextComponent& operator=(const TextComponent & other) = delete;
		TextComponent& operator=(TextComponent && other) = delete;

		void SetText(const std::string& text);
		void SetFont(const std::shared_ptr<Font>& font);
		void SetColor(SDL_Color color);

		std::shared_ptr<Texture2D> GenerateTexture();

	private:
		bool m_NeedsUpdate{ true };
		std::string m_Text{"Sample Text"};
		SDL_Color m_Color{255, 255 , 255, 255};

		std::shared_ptr<Font> m_pFont;

		std::weak_ptr<Texture2D> m_pLastGeneratedTexture;
	};
}
