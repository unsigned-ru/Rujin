#pragma once
#include "Component.h"

namespace rujin
{
	class Font;

	class FontComponent final : public Component
	{
	public:
		explicit FontComponent(const std::weak_ptr<GameObject> gameObject);

		void SetFont(const std::shared_ptr<Font> font);
		std::weak_ptr<Font> GetFont() const;

	private:
		std::shared_ptr<Font> m_Font{};
	};
}

