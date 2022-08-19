#ifndef TEXTURE_RENDER_COMPONENT_H
#define TEXTURE_RENDER_COMPONENT_H
#include "Component.h"

namespace rujin
{
	class Texture;

	class TextureRenderComponent : public Component
	{
	public:
		explicit TextureRenderComponent() = default;
		explicit TextureRenderComponent(const std::shared_ptr<Texture>& texture, const glm::vec2& pivot = { .5f, .5f }, const Recti& sourceRect = {});

		void LateStart() override;
		void Draw() const override;

		void SetTexture(const std::shared_ptr<Texture>& texture);
		void SetSourceRect(const Recti& sourceRect = {});
		void SetPivot(const glm::vec2& pivot);

		void SetFlippedX(bool flipped);
		void SetFlippedY(bool flipped);
		bool IsFlippedX() const;
		bool IsFlippedY() const;

		const glm::vec2& GetPivot() const { return m_Pivot; }

	private:
		std::shared_ptr<Texture> m_pTexture{};
		glm::vec2 m_Pivot = { .5f, .5f };
		Recti m_SourceRect{};
		bool m_IsFlippedX{false};
		bool m_IsFlippedY{false};
	};
}

#endif