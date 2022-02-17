#pragma once
#include "Texture2D.h"
#include "TransformComponent.h"

namespace rujin
{
	class Font;
	class Texture2D;

	class TextComponent final : public Component
	{
	public:
		explicit TextComponent(const std::weak_ptr<GameObject> gameObject);
		~TextComponent() override = default;
		TextComponent(const TextComponent & other) = delete;
		TextComponent(TextComponent && other) = delete;
		TextComponent& operator=(const TextComponent & other) = delete;
		TextComponent& operator=(TextComponent && other) = delete;

		void Update() override;

		void SetText(const std::string& text);
		void SetFont(const std::weak_ptr<Font> font);
		void SetPosition(float x, float y);
		void SetColor(SDL_Color color);

		void GenerateTextTexture();

		std::weak_ptr<Texture2D> GetTexture() const;

	private:
		bool m_NeedsUpdate{true};
		std::string m_Text{"Sample Text"};
		std::weak_ptr<Font> m_Font{};
		SDL_Color m_Color{255, 255 , 255, 255};
		std::shared_ptr<Texture2D> m_TextTexture{};
	};
}
