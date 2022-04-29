#include "RujinPCH.h"
#include "ResourceManager.h"

#include "Renderer.h"

#include "Texture2D.h"
#include "Font.h"

#include <SDL_image.h>
#include <SDL_ttf.h>

#include "AudioClip.h"

void rujin::ResourceManager::Init(const std::string& dataPath)
{
	m_DataPath = dataPath;

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

}

std::shared_ptr<rujin::Texture2D> rujin::ResourceManager::LoadTexture(const std::string& file)
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

	auto texture = IMG_LoadTexture(Renderer::GetInstance().GetSDLRenderer(), fullPath.c_str());

	if (texture == nullptr) 
		throw std::runtime_error(std::string("Failed to load texture: ") + SDL_GetError());

	auto rv = std::make_shared<Texture2D>(texture);

	//remember we loaded in this texture, store a weak_ptr
	m_TextureDict[file] = rv;

	return rv;
}

std::shared_ptr<rujin::Font> rujin::ResourceManager::LoadFont(const std::string& file, uint32_t size)
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

std::shared_ptr<rujin::AudioClip> rujin::ResourceManager::LoadAudio(const std::string& file)
{
	const auto fullPath = m_DataPath + file;

	//check wether there is an ID associated with this path
	//(Has this audio clip been loaded before and linked to an ID)
	const auto pathIt = m_AudioRememberedPaths.find(file);
	if (pathIt != m_AudioRememberedPaths.end())
	{
		//We remember loading this path before, get the ID and see if the Clip is expired

		const auto id = pathIt->second;
		const auto clipIt = m_AudioClipDict.find(id);

		//if we find a clip with that ID
		if (clipIt != m_AudioClipDict.end())
		{
			if (!clipIt->second.expired())
			{
				//our resource still exists and is loaded.
				//return a shared_ptr instance of our non expired weak_ptr
				return clipIt->second.lock();
			}
			
			//else
			//weak_ptr expired, load resource
			Mix_Chunk* pChunk = Mix_LoadWAV(fullPath.c_str());
			if (!pChunk)
				throw std::runtime_error(std::string("Failed to load Audio: ") + Mix_GetError());

			//hand it the same ID as the expired one
			auto rv = std::shared_ptr<AudioClip>(new AudioClip(id, pChunk));

			m_AudioClipDict.at(id) = rv;
			return rv;

		}

		//we did not find a clip with that ID, construct a new one! 
	}

	//Load resource
	Mix_Chunk* pChunk = Mix_LoadWAV(fullPath.c_str());
	if (!pChunk)
		throw std::runtime_error(std::string("Failed to load Audio: ") + Mix_GetError());

	//Create AudioClip
	auto rv = std::shared_ptr<AudioClip>(new AudioClip(pChunk));
	const clip_id id = rv->GetId();

	//Add it to the dictionary
	m_AudioClipDict.at(id) = rv;

	//Remember the path
	m_AudioRememberedPaths.at(file) = id;

	return rv;
}


