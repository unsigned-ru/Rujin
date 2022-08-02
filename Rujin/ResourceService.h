#ifndef RESOURCE_SERVICE_H
#define RESOURCE_SERVICE_H

#include "Service.h"

#include <memory>
#include <string>

namespace rujin
{
	class Texture;
	class Font;

	class ResourceService : public Service
	{
	public:
		ResourceService() = default;
		~ResourceService() override = default;

		ResourceService(const ResourceService&) = delete;
		ResourceService(ResourceService&&) noexcept = delete;
		ResourceService& operator=(const ResourceService&) = delete;
		ResourceService& operator=(ResourceService&&) noexcept = delete;

		virtual std::shared_ptr<Texture> LoadTexture(const std::string& file) = 0;
		virtual std::shared_ptr<Font> LoadFont(const std::string& file, uint32_t size) = 0;

		virtual std::string GetDataPath() const { return m_DataPath; }

	protected:
		std::string m_DataPath{};
	};

}


#endif // Include Guard: RESOURCE_SERVICE_H
