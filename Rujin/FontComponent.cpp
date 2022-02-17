#include "RujinPCH.h"
#include "FontComponent.h"

rujin::FontComponent::FontComponent(const std::weak_ptr<GameObject> gameObject)
	: Component(gameObject)
{
}

void rujin::FontComponent::SetFont(const std::shared_ptr<Font> font)
{
	m_Font = font;
}

std::weak_ptr<rujin::Font> rujin::FontComponent::GetFont() const
{
	return m_Font;
}
