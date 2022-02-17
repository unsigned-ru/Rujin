#pragma once
#include "Component.h"

namespace rujin
{
	class Texture2D;

	class TextureComponent : public Component
	{
	public:
		explicit TextureComponent(const std::weak_ptr<GameObject> gameObject);

		void SetTexture(std::shared_ptr<Texture2D> texture);
		std::weak_ptr<Texture2D> GetTexture() const;

	private:
		std::shared_ptr<Texture2D> m_Texture{};
	};
}

