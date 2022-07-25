#pragma once
struct SDL_Texture;
namespace rujin
{
	//TODO: is not RAII.
	/**
	 * Simple RAII wrapper for an SDL_Texture
	 */
	class Texture final
	{
	public:
		explicit Texture(GLuint textureId, const glm::ivec2& size);
		
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
	};
}
