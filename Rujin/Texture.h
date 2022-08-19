#ifndef TEXTURE_H
#define TEXTURE_H
#include <SDL_ttf.h>

namespace rujin
{
	/**
	 * Simple RAII wrapper for an OpenGL Texture
	 */
	class Texture final
	{
	public:
		explicit Texture(const std::string& path);
		explicit Texture(const std::string& text, TTF_Font* pFont);
		explicit Texture(const SDL_Surface* pSurface);

		~Texture();

		Texture(const Texture &) = delete;
		Texture(Texture &&) = delete;
		Texture & operator= (const Texture &) = delete;
		Texture & operator= (const Texture &&) = delete;

		GLuint GetId() const { return m_TextureId; };
		const glm::ivec2& GetSize() const { return m_Size; }

	protected:
		GLuint m_TextureId{};
		glm::ivec2 m_Size{};

		void CreateFromImage(const std::string& path);
		void CreateFromString(const std::string& text, TTF_Font* pFont);
		void CreateFromSurface(const SDL_Surface* pSurface);
	};
}

#endif