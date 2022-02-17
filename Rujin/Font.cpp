#include "RujinPCH.h"
#include <SDL_ttf.h>
#include "Font.h"

rujin::Font::Font(const std::string& fullPath, const uint32_t size)
	: m_Font(nullptr)
	, m_Size(size)
{
	m_Font = TTF_OpenFont(fullPath.c_str(), size);
	if (m_Font == nullptr) 
	{
		throw std::runtime_error(std::string("Failed to load font: ") + SDL_GetError());
	}
}

rujin::Font::~Font()
{
	TTF_CloseFont(m_Font);
}

uint32_t rujin::Font::GetSize() const
{
	return m_Size;
}


TTF_Font* rujin::Font::GetFont() const {
	return m_Font;
}
