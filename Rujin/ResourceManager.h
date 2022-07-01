#pragma once
#include "Singleton.h"

#include <unordered_map>

namespace rujin
{
	//Motivations for using smart pointers:
	//	- Removing possibility of memory leaks for the engine users
	//	- It's a good use of smart_pointers
	//	- A "more manual" Resource management system can be layered on top of this if the engine
	//	  user wishes for that.
	//	- Prevents possible bugs in threading, If the owning class gets destroyed,
	//	  and the resource is still utilized in a thread, it will not be destroyed
	//	  until the thread is done with the resource (preventing "what the heck is going on" bugs)

	//Downsides:
	//	- Memory usage is slightly increased
	//	- Smart pointers come with a cost, have to be careful with passing them around
	//	  https://stackoverflow.com/questions/2502394/the-cost-of-passing-by-shared-ptr

	class Texture2D;
	class Font;
	class AudioClip;

	class ResourceManager final : public Singleton<ResourceManager>
	{
	public:
		void Init(const std::string& dataPath);

		std::shared_ptr<Texture2D> LoadTexture(const std::string& file);
		std::shared_ptr<Font> LoadFont(const std::string& file, uint32_t size);

		std::string GetDataPath() const;

	private:
		friend class Singleton<ResourceManager>;
		ResourceManager() = default;

		std::unordered_map<std::string, std::weak_ptr<Font>> m_FontDict;
		std::unordered_map<std::string, std::weak_ptr<Texture2D>> m_TextureDict;

		std::string m_DataPath{};
	};
}
