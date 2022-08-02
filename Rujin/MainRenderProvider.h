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

		WindowContext& m_WindowInfo;
		SDL_GLContext m_pContext;
	};
}



#endif // Include Guard: MAIN_RENDER_PROVIDER_H
