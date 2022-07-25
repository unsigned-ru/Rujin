#pragma once
#include "Component.h"

#include "Texture.h"

namespace rujin
{
	class TextureRenderComponent;
	class Font;

	class TextComponent final : public Component
	{
	public:
		explicit TextComponent() = default;
		explicit TextComponent(const std::shared_ptr<Font>& pFont, const SDL_Color& color = SDL_Color{255, 255, 255, 255});

		~TextComponent() override = default;
		TextComponent(const TextComponent & other) = delete;
		TextComponent(TextComponent && other) = delete;
		TextComponent& operator=(const TextComponent & other) = delete;
		TextComponent& operator=(TextComponent && other) = delete;

		void Start() override;

		void SetText(const std::string& text);
		void SetFont(const std::shared_ptr<Font>& font);
		void SetColor(SDL_Color color);

		std::shared_ptr<Texture> GenerateTexture();
		bool IsDirty() const;

	private:
		bool m_IsDirty{ true };
		std::string m_Text{"Sample Text"};
		SDL_Color m_Color{255, 255 , 255, 255};

		std::shared_ptr<Font> m_pFont;
		std::weak_ptr<Texture> m_pLastGeneratedTexture;
	};
}
