#pragma once
#include "Service.h"

namespace rujin
{
	struct Rectf;
	struct Transform;
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
		virtual void RenderTexture(const Texture& texture, const Transform& transform, const glm::vec2& pivot, const Rectf* srcRect = nullptr) const = 0;
	};
}
