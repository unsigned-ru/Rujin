#pragma once
#include <unordered_map>
#include "Singleton.h"

namespace rujin
{
	class Texture2D;
	class Font;
	class ResourceManager final : public Singleton<ResourceManager>
	{
	public:
		void Init(const std::string& dataPath);
		std::shared_ptr<Texture2D> LoadTexture(const std::string& file);
		std::shared_ptr<Font> LoadFont(const std::string& file, uint32_t size);
	private:
		friend class Singleton<ResourceManager>;
		ResourceManager() = default;

		std::unordered_map<std::string, std::weak_ptr<Font>> m_FontDict;
		std::unordered_map<std::string, std::weak_ptr<Texture2D>> m_TextureDict;

		std::string m_DataPath{};
	};
}
