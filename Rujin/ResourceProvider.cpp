#include "RujinPCH.h"
#include "ResourceProvider.h"

#include "MainRenderProvider.h"

#include "Texture.h"
#include "Font.h"

#include <SDL_image.h>
#include <SDL_ttf.h>

#include "Rujin.h"

rujin::ResourceProvider::ResourceProvider()
{
	LOG_DEBUG("Initializing Resource Manager...");
	const auto& initParams = Rujin::Get()->GetInitParams();

	m_DataPath = initParams.resourcePath;

	// load support for png and jpg, this takes a while!

	if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG)
	{
		throw std::runtime_error(std::string("Failed to load support for png's: ") + SDL_GetError());
	}

	if ((IMG_Init(IMG_INIT_JPG) & IMG_INIT_JPG) != IMG_INIT_JPG)
	{
		throw std::runtime_error(std::string("Failed to load support for jpg's: ") + SDL_GetError());
	}

	if (TTF_Init() != 0)
	{
		throw std::runtime_error(std::string("Failed to load support for fonts: ") + SDL_GetError());
	}

	LOG_DEBUG("Resource Manager Initialized!");
}

//TODO: make RAII instead.
std::shared_ptr<rujin::Texture> rujin::ResourceProvider::LoadTexture(const std::string& file)
{
	const auto fullPath = m_DataPath + file;

	//check if we remember loading this texture before
	const auto it = m_TextureDict.find(file);
	if (it != m_TextureDict.end() && !it->second.expired())
	{
		//we found a remembered weak pointer to the texture
		//weak pointer is still valid
		//we can return a shared pointer to the already existing resource, we don't have to load in a new one.
		return it->second.lock();
	}

	//we haven't loaded in the texture before.

	// Load image at specified path
	
	SDL_Surface* pSurface = IMG_Load(fullPath.c_str());
	if (pSurface == nullptr)
	{
		LOG_ERROR_("Error when calling IMG_Load : {}", SDL_GetError());
		return nullptr;
	}


	// Get pixel format information and translate to OpenGl format
	GLenum pixelFormat{ GL_RGB };
	switch (pSurface->format->BytesPerPixel)
	{
	case 3:
		if (pSurface->format->Rmask == 0x000000ff)
		{
			pixelFormat = GL_RGB;
		}
		
		break;
	case 4:
		if (pSurface->format->Rmask == 0x000000ff)
		{
			pixelFormat = GL_RGBA;
		}
		
		break;
	default:
		LOG_ERROR_("unknown pixel format, BytesPerPixel: {}\nUse 32 bit or 24 bit images.", pSurface->format->BytesPerPixel);
		return nullptr;
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

	//make the texture obj
	auto rv = std::make_shared<Texture>(texId, glm::ivec2{ pSurface->w, pSurface->h });

	// Free loaded surface
	SDL_FreeSurface(pSurface);

	//remember we loaded in this texture, store a weak_ptr
	m_TextureDict[file] = rv;

	return rv;
}

std::shared_ptr<rujin::Font> rujin::ResourceProvider::LoadFont(const std::string& file, uint32_t size)
{
	const auto fullPath = m_DataPath + file;

	//check if we remember loading this font with that size before
	const auto it = m_FontDict.find(file + std::to_string(size));

	if (it != m_FontDict.end() && !it->second.expired())
	{
		//we found a remembered weak pointer to the font
		//weak pointer is still valid
		//we can return a shared pointer to the already existing resource, we don't have to load in a new one.
		return it->second.lock();
	}

	//load in new font and remember we did.
	const auto rv = std::make_shared<Font>(fullPath, size);
	m_FontDict[file + std::to_string(size)] = rv;

	return rv;
}