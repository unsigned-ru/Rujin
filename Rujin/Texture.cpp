#include "RujinPCH.h"
#include "Texture.h"

#include <SDL_image.h>

rujin::Texture::Texture(const std::string& path)
{
	CreateFromImage(path);
}

rujin::Texture::Texture(const std::string& text, TTF_Font* pFont)
{
	CreateFromString(text, pFont);
}

rujin::Texture::Texture(const SDL_Surface* pSurface)
{
	CreateFromSurface(pSurface);
}

rujin::Texture::~Texture()
{
	glDeleteTextures(1, &m_TextureId);
}

void rujin::Texture::CreateFromImage(const std::string& path)
{
	// Load image at specified path
	const SDL_Surface* pLoadedSurface = IMG_Load(path.c_str());
	ASSERT(pLoadedSurface)

	CreateFromSurface(pLoadedSurface);
}

void rujin::Texture::CreateFromString(const std::string& text, TTF_Font* pFont)
{
	ASSERT(pFont);

	//always white to make sure we can just change color by changing color before rendering
	SDL_Surface* pLoadedSurface = TTF_RenderText_Blended(pFont, text.c_str(), {255, 255, 255, 255});
	ASSERT_MSG(pLoadedSurface, "Failed to generate texture from font and text.");

	// Copy to video memory
	CreateFromSurface(pLoadedSurface);

	// Free loaded surface
	SDL_FreeSurface(pLoadedSurface);
}

void rujin::Texture::CreateFromSurface(const SDL_Surface* pSurface)
{
	ASSERT(pSurface);

	// Get pixel format information and translate to OpenGl format
	GLenum pixelFormat{ GL_RGB };
	switch (pSurface->format->BytesPerPixel)
	{
	case 3:
		pixelFormat = GL_RGB;
		break;
	case 4:
		pixelFormat = GL_RGBA;
		break;
	default:
		LOG_ERROR_("unknown pixel format, BytesPerPixel: {}\nUse 32 bit or 24 bit images.", pSurface->format->BytesPerPixel);
	}

	//Generate an array of textures.  We only want one texture (one element array), so trick
	//it by treating "texture" as array of length one.
	GLuint texId;
	glGenTextures(1, &texId);

	//Select (bind) the texture we just generated as the current 2D texture OpenGL is using/modifying.
	//All subsequent changes to OpenGL's texturing state for 2D textures will affect this texture.
	glBindTexture(GL_TEXTURE_2D, texId);

	// check for errors. Can happen if a texture is created while a static pointer is being initialized, even before the call to the main function.
	GLenum e = glGetError();
	if (e != GL_NO_ERROR)
		LOG_ERROR_(R"(Texture::CreateFromSurface, error binding textures, Error id = {}
			Can happen if a texture is created before performing the initialization code (e.g. a static Texture object)
			There might be a white rectangle instead of the image.)", e);

	// Specify the texture's data.  
	glTexImage2D
	(
		GL_TEXTURE_2D,
		0,
		GL_RGBA,
		pSurface->w,
		pSurface->h,
		0,
		pixelFormat,
		GL_UNSIGNED_BYTE,
		pSurface->pixels
	);

	//Scaling behavior
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//Set member vars
	m_TextureId = texId;
	m_Size = glm::ivec2(pSurface->w, pSurface->h);
}
