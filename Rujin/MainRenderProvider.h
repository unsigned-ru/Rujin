#ifndef MAIN_RENDER_PROVIDER_H
#define MAIN_RENDER_PROVIDER_H

#include "RenderService.h"

namespace rujin
{
	struct Rectf;
	struct Transform;
	struct WindowContext;

	namespace settings
	{
		struct InitParams;
	}

	class Texture;

	class MainRenderProvider final : public RenderService
	{
	public:
		MainRenderProvider();
		~MainRenderProvider() override;

		MainRenderProvider(const MainRenderProvider&) = delete;
		MainRenderProvider(MainRenderProvider&&) noexcept = delete;
		MainRenderProvider& operator=(const MainRenderProvider&) = delete;
		MainRenderProvider& operator=(MainRenderProvider&&) noexcept = delete;

		void Render() const override;
		void RenderTexture(const Texture& texture, const Transform& transform, const glm::vec2& pivot, const Rectf* srcRect = nullptr) const override;

#pragma region Primitive Drawing
		void SetColor(const glm::vec4& color) const override;

		void DrawPoint(const glm::vec2& p, float pointSize = 1.0f) const override;

		void DrawLine(const glm::vec2& p1, const glm::vec2& p2, float lineWidth = 1.0f) const override;

		void DrawRect(const Rectf& rect, float lineWidth = 1.0f) const override;
		void FillRect(const Rectf& rect) const override;

		//void DrawEllipse(const Ellipsef& ellipse, float lineWidth = 1.0f) override;
		//void FillEllipse(const Ellipsef& ellipse) override;

		//void DrawArc(const Point2f& center, float radX, float radY, float fromAngle, float tillAngle, float lineWidth = 1.0f) override;
		//void FillArc(const Point2f& center, float radX, float radY, float fromAngle, float tillAngle) override;

		//void DrawPolygon(const std::vector<Point2f>& vertices, bool closed = true, float lineWidth = 1.0f) override;
		//void DrawPolygon(const Point2f* pVertices, size_t nrVertices, bool closed = true, float lineWidth = 1.0f) override;

		//void FillPolygon(const std::vector<Point2f>& vertices) override;
		//void FillPolygon(const Point2f* pVertices, size_t nrVertices) override;
#pragma endregion

		WindowContext& m_WindowInfo;
		SDL_GLContext m_pContext;
	};
}



#endif // Include Guard: MAIN_RENDER_PROVIDER_H
