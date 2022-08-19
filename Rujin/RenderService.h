#ifndef RENDER_SERVICE_H
#define RENDER_SERVICE_H

#include "Service.h"
#include "Transform.h"

namespace rujin
{
	class Texture;

	class RenderService : public Service
	{
	public:
		RenderService() = default;
		~RenderService() override = default;

		RenderService(const RenderService&) = delete;
		RenderService(RenderService&&) noexcept = delete;
		RenderService& operator=(const RenderService&) = delete;
		RenderService& operator=(RenderService&&) noexcept = delete;

		virtual void Render() const = 0;
		virtual void RenderTexture(const Texture& texture, const Position& position, const Rotation rot = { 0.f }, const Scale& scale = { 1.f, 1.f }, const glm::vec2& pivot = { 0.5f, 0.5f }, const Recti* srcRect = nullptr, bool isFlippedX = false, bool isFlippedY = false) const = 0;
		void RenderTexture(const Texture& texture, const Transform& transform, const glm::vec2& pivot = { 0.5f, 0.5f }, const Recti* srcRect = nullptr, bool isFlippedX = false, bool isFlippedY = false) const
		{
			RenderTexture(texture, transform.GetPosition(), transform.GetRotation(), transform.GetScale(), pivot, srcRect, isFlippedX, isFlippedY);
		}

#pragma region Primitive Drawing

		virtual void SetColor(const glm::vec4& color = {1.f, 1.f, 1.f, 1.f}) const = 0;

		virtual void DrawPoint(const glm::vec2& p, float pointSize = 1.0f) const = 0;

		virtual void DrawLine(const glm::vec2& p1, const glm::vec2& p2, float lineWidth = 1.0f) const = 0;

		virtual void DrawRect(const Rectf& rect, float lineWidth = 1.0f) const = 0;
		virtual void FillRect(const Rectf& rect) const = 0;

		//virtual void DrawEllipse(const Ellipsef& ellipse, float lineWidth = 1.0f) = 0;
		//virtual void FillEllipse(const Ellipsef& ellipse) = 0;

		//virtual void DrawArc(const Point2f& center, float radX, float radY, float fromAngle, float tillAngle, float lineWidth = 1.0f) = 0;
		//virtual void FillArc(const Point2f& center, float radX, float radY, float fromAngle, float tillAngle) = 0;

		//virtual void DrawPolygon(const std::vector<Point2f>& vertices, bool closed = true, float lineWidth = 1.0f) = 0;
		//virtual void DrawPolygon(const Point2f* pVertices, size_t nrVertices, bool closed = true, float lineWidth = 1.0f) = 0;

		//virtual void FillPolygon(const std::vector<Point2f>& vertices) = 0;
		//virtual void FillPolygon(const Point2f* pVertices, size_t nrVertices) = 0;
#pragma endregion

	};
}


#endif // Include Guard: RENDER_SERVICE_H
