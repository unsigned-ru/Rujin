#pragma once
#include "Texture2D.h"

namespace rujin
{
	/**
	 * \brief RAII wrapper for a texture who's SDL_Texture can change at runtime, automatically frees when a new texture gets set.
	 * \note Dynamic Textures do not get handled by the Resource Manager.
	 */
	class DynamicTexture2D final : public Texture2D
	{
	public:
		explicit DynamicTexture2D(SDL_Texture* texture);

		void SetSDLTexture(SDL_Texture* texture);
		~DynamicTexture2D() override;
	};
}

