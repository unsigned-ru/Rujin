#pragma once
#include "DynamicTexture2D.h"
#include "TransformComponent.h"

namespace rujin
{
	class Font;

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
		void SetColor(SDL_Color color);

		std::weak_ptr<DynamicTexture2D> GetTexture() const;

	private:
		void GenerateTextTexture();

		bool m_NeedsUpdate{true};
		std::string m_Text{"Sample Text"};
		std::weak_ptr<Font> m_Font{};
		SDL_Color m_Color{255, 255 , 255, 255};
		std::shared_ptr<DynamicTexture2D> m_TextTexture{std::make_shared<DynamicTexture2D>(nullptr)};
	};
}
