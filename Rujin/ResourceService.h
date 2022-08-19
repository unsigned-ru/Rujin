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

		/**
		 * \brief Gets string texture
		 * \note If it doesn't exist, it will load/generate the texture.
		 * \note If specified fon doesn't exist, it will try and load the font.
		 */
		virtual std::shared_ptr<Texture> GetStringTexture(const std::string& text, const std::string& fontFile, uint32_t fontSize);

		/**
		 * \brief Gets string texture
		 * \note If it doesn't exist, it will load/generate the texture.
		 */
		virtual std::shared_ptr<Texture> GetStringTexture(const std::string& text, std::shared_ptr<Font>& pFont);

		/**
		 * \brief Gets texture
		 * \note If it doesn't exist, it will load/generate the texture.
		 */
		virtual std::shared_ptr<Texture> GetTexture(const std::string& file);

		/**
		 * \brief Gets font
		 * \note If it doesn't exist, it will load/generate the texture.
		 */
		virtual std::shared_ptr<Font> GetFont(const std::string& file, uint32_t size);

		virtual std::string GetDataPath() const { return m_DataPath; }

	protected:
		std::string m_DataPath{};
	};

}


#endif // Include Guard: RESOURCE_SERVICE_H
