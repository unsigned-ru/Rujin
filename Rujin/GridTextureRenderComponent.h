#ifndef GRID_TEXTURE_RENDER_COMPONENT_H
#define GRID_TEXTURE_RENDER_COMPONENT_H
#include "TextureRenderComponent.h"

namespace rujin
{
	class GridTextureRenderComponent final : public TextureRenderComponent
	{
	public:
		explicit GridTextureRenderComponent(const glm::ivec2& gridDimensions, const glm::vec2& cellOffset, const std::shared_ptr<Texture>& texture, const glm::vec2& texturePivot = { .5f, .5f }, const Recti& sourceRect = {}, bool growLeft = false, bool growDown = false);

		void SetGridDimensions(const glm::ivec2& newDimensions);
		const glm::ivec2& GetGridDimensions() const;

		void SetCellOffset(const glm::vec2& newCellOffset);
		const glm::vec2& GetCellOffset() const;

		void Draw() const override;

	private:
		glm::ivec2 m_GridDimensions;
		glm::vec2 m_CellOffset;

		bool m_GrowLeft;
		bool m_GrowDown;
	};
}


#endif // Include Guard: GRID_TEXTURE_RENDER_COMPONENT_H
