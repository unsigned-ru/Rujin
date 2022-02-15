#include "RujinPCH.h"
#include "Texture2D.h"

rujin::Texture2D::Texture2D(SDL_Texture* pTexture)
	: m_pTexture(pTexture)
{}

rujin::Texture2D::~Texture2D()
{
	SDL_DestroyTexture(m_pTexture);
}

SDL_Texture* rujin::Texture2D::GetSDLTexture() const
{
	return m_pTexture;
}