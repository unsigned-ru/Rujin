#pragma once
#include "Singleton.h"

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

	class Renderer final : public Singleton<Renderer>
	{
	public:
		Renderer(const Renderer&) = delete;
		Renderer(Renderer&&) noexcept = delete;
		Renderer& operator=(const Renderer&) = delete;
		Renderer& operator=(Renderer&&) noexcept = delete;

		void Render() const;
		void RenderTexture(const Texture& texture, const Transform& transform, const glm::vec2& pivot, const Rectf* srcRect = nullptr) const;

	private:
		friend class Singleton<Renderer>;
		Renderer();
		~Renderer() override;

		void Initialize() override;

		WindowContext& m_WindowInfo;
		SDL_GLContext m_pContext;
	};
}

