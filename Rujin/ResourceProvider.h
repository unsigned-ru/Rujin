#ifndef RESOURCE_PROVIDER_H
#define RESOURCE_PROVIDER_H


#include <unordered_map>

#include "ResourceService.h"

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

#pragma region Forward Declarations
	class Texture;
	class Font;
	class AudioClip;

	namespace settings
	{
		struct InitParams;
	}
#pragma endregion

	class ResourceProvider final : public ResourceService
	{
	public:
		ResourceProvider();
		~ResourceProvider() override = default;

		ResourceProvider(const ResourceProvider&) = delete;
		ResourceProvider(ResourceProvider&&) noexcept = delete;
		ResourceProvider& operator=(const ResourceProvider&) = delete;
		ResourceProvider& operator=(ResourceProvider&&) noexcept = delete;

		std::shared_ptr<Texture> GetStringTexture(const std::string& text, const std::string& fontFile, uint32_t fontSize) override;
		std::shared_ptr<Texture> GetStringTexture(const std::string& text, std::shared_ptr<Font>& pFont) override;
		std::shared_ptr<Texture> GetTexture(const std::string& file) override;
		std::shared_ptr<Font> GetFont(const std::string& file, uint32_t size) override;
		

	private:
		std::unordered_map<std::string, std::weak_ptr<Font>> m_FontDict{};
		std::unordered_map<std::string, std::weak_ptr<Texture>> m_TextureDict{};
		std::unordered_map<std::string, std::weak_ptr<Texture>> m_StringTextureDict{};
	};
}


#endif // Include Guard: RESOURCE_PROVIDER_H
