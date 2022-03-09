#pragma once
struct SDL_Texture;
namespace rujin
{
	/**
	 * Simple RAII wrapper for an SDL_Texture
	 */
	class Texture2D
	{
	public:
		explicit Texture2D(SDL_Texture* pTexture);
		explicit Texture2D() = default;
		virtual ~Texture2D();

		Texture2D(const Texture2D &) = delete;
		Texture2D(Texture2D &&) = delete;
		Texture2D & operator= (const Texture2D &) = delete;
		Texture2D & operator= (const Texture2D &&) = delete;

		SDL_Texture* GetSDLTexture() const;
		
	protected:
		SDL_Texture* m_pTexture{nullptr};
	};
}
