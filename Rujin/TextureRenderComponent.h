#pragma once
#include <variant>

#include "Component.h"
namespace rujin
{
	class Texture2D;

	class TextureRenderComponent final : public Component
	{
	public:
		explicit TextureRenderComponent() = default;

		void Render() const override;
		void SetTexture(const std::shared_ptr<Texture2D>& texture);

	private:
		std::shared_ptr<Texture2D> m_pTexture{};
	};
}

