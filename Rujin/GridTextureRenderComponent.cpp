#include "RujinPCH.h"
#include "GridTextureRenderComponent.h"

#include "GameObject.h"
#include "RenderService.h"
#include "ServiceLocator.h"

rujin::GridTextureRenderComponent::GridTextureRenderComponent(const glm::ivec2& gridDimensions, const glm::vec2& cellOffset, const std::shared_ptr<Texture>& texture, const glm::vec2& texturePivot, const Recti& sourceRect, bool growLeft, bool growDown)
	: TextureRenderComponent{texture, texturePivot, sourceRect}
	, m_GridDimensions(gridDimensions)
	, m_CellOffset(cellOffset)
	, m_GrowLeft(growLeft)
	, m_GrowDown(growDown)
{
}

void rujin::GridTextureRenderComponent::SetGridDimensions(const glm::ivec2& newDimensions)
{
	m_GridDimensions = newDimensions;
}

const glm::ivec2& rujin::GridTextureRenderComponent::GetGridDimensions() const
{
	return m_GridDimensions;
}

void rujin::GridTextureRenderComponent::SetCellOffset(const glm::vec2& newCellOffset)
{
	m_CellOffset = newCellOffset;
}

const glm::vec2& rujin::GridTextureRenderComponent::GetCellOffset() const
{
	return m_CellOffset;
}

void rujin::GridTextureRenderComponent::Draw() const
{
	//DO NOT CALL BASE CLASS, WE FULLY OVERRIDE DRAWING.

	const auto& renderer = ServiceLocator::GetService<RenderService>();

	const Transform& transform = GameObject()->GetTransform();
	const Position& startPos = transform.GetPosition();
	const Rotation rot = transform.GetRotation();
	const Scale& scale = transform.GetScale();

	for (int32_t y = 0; y < m_GridDimensions.y; ++y)
	{
		for (int32_t x = 0; x < m_GridDimensions.x; ++x)
		{
			Position pos;

			if (m_GrowLeft)
				pos.x = startPos.x - x * (m_SourceRect.width * scale.x + m_CellOffset.x);
			else
				pos.x = startPos.x + x * (m_SourceRect.width * scale.x + m_CellOffset.x);

			if (m_GrowDown)
				pos.y = startPos.y - y * (m_SourceRect.height * scale.y + m_CellOffset.y);
			else
				pos.y = startPos.y + y * (m_SourceRect.height * scale.y + m_CellOffset.y);

			renderer.RenderTexture
			(
				*m_pTexture.get(),
				pos,
				rot,
				scale,
				m_Pivot,
				&m_SourceRect,
				m_IsFlippedX,
				m_IsFlippedY
			);
		}
	}

}
