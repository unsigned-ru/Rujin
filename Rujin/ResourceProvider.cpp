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

std::shared_ptr<rujin::Texture> rujin::ResourceProvider::GetStringTexture(const std::string& text, const std::string& fontFile, uint32_t fontSize)
{
	//check if we remember loading this texture before
	const std::string keyValue = fmt::format("{}_{}_{}", text, fontFile, fontSize);
	if (const auto it = m_StringTextureDict.find(keyValue); it != m_StringTextureDict.end() && !it->second.expired())
	{
		//we found a remembered weak pointer to the texture
		//weak pointer is still valid
		//we can return a shared pointer to the already existing resource, we don't have to load in a new one.
		return it->second.lock();
	}

	//get font first...
	auto pFont = GetFont(m_DataPath + fontFile, fontSize);

	//Get by font
	return GetStringTexture(text, pFont);
}

std::shared_ptr<rujin::Texture> rujin::ResourceProvider::GetStringTexture(const std::string& text, std::shared_ptr<Font>& pFont)
{
	//check if we remember loading this texture before
	const std::string keyValue = fmt::format("{}_{}_{}", text, pFont->GetFullPath(), pFont->GetSize());

	if (const auto it = m_StringTextureDict.find(keyValue); it != m_StringTextureDict.end() && !it->second.expired())
	{
		//we found a remembered weak pointer to the texture
		//weak pointer is still valid
		//we can return a shared pointer to the already existing resource, we don't have to load in a new one.
		return it->second.lock();
	}

	//we haven't loaded in this texture before, or it is expired.
	auto rv = std::make_shared<Texture>(text, pFont->GetFont());

	//remember we loaded in this texture...
	m_StringTextureDict[keyValue] = rv;

	return rv;
}

std::shared_ptr<rujin::Texture> rujin::ResourceProvider::GetTexture(const std::string& file)
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
	//Create the texture
	auto rv = std::make_shared<Texture>(fullPath);

	//remember we loaded in this texture, store a weak_ptr
	m_TextureDict[file] = rv;

	return rv;
}

std::shared_ptr<rujin::Font> rujin::ResourceProvider::GetFont(const std::string& file, uint32_t size)
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