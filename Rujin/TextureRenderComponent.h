#pragma once
#include "Component.h"

namespace rujin
{
	class Texture;

	class TextureRenderComponent final : public Component
	{
	public:
		explicit TextureRenderComponent() = default;
		explicit TextureRenderComponent(const std::shared_ptr<Texture>& texture, const glm::vec2& pivot = { .5f, .5f });

		void LateStart() override;
		void Draw() const override;

		void SetTexture(const std::shared_ptr<Texture>& texture);
		void SetPivot(const glm::vec2& pivot);

		const glm::vec2& GetPivot() const { return m_Pivot; }

	private:
		std::shared_ptr<Texture> m_pTexture{};
		glm::vec2 m_Pivot = { .5f, .5f };
	};
}

