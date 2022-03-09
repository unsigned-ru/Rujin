#include "RujinPCH.h"
#include "DynamicTexture2D.h"

rujin::DynamicTexture2D::DynamicTexture2D(SDL_Texture* texture)
	: Texture2D(texture)
{
}

void rujin::DynamicTexture2D::SetSDLTexture(SDL_Texture* texture)
{
	//destroy old texture
	SDL_DestroyTexture(m_pTexture);

	//set new one
	m_pTexture = texture;
}

rujin::DynamicTexture2D::~DynamicTexture2D() = default;
