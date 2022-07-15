#include "RujinPCH.h"
#include "Renderer.h"

#include "imgui.h"
#include "Rujin.h"
#include "backends/imgui_impl_opengl2.h"
#include "backends/imgui_impl_sdl.h"

#include "SceneManager.h"

#include "Texture2D.h"

int GetOpenGLDriverIndex()
{
	auto openglIndex = -1;
	const auto driverCount = SDL_GetNumRenderDrivers();
	for (auto i = 0; i < driverCount; i++)
	{
		SDL_RendererInfo info;
		if (!SDL_GetRenderDriverInfo(i, &info))
			if (!strcmp(info.name, "opengl"))
				openglIndex = i;
	}
	return openglIndex;
}

void rujin::Renderer::Init(SDL_Window * window, rujin::settings::InitializationParameters& params)
{
	LOG_DEBUG("Initializing OpenGL & SDL Renderer...");

	m_Window = window;
	m_Renderer = SDL_CreateRenderer(window, GetOpenGLDriverIndex(), SDL_RENDERER_ACCELERATED);
	if (m_Renderer == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
	}

	//Set VSync
	SDL_GL_SetSwapInterval(static_cast<int>(params.vsyncMode));
	
	//initialize ImGui
	LOG_DEBUG("Hooking ImGUI...");
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplSDL2_InitForOpenGL(window, SDL_GL_GetCurrentContext());
	ImGui_ImplOpenGL2_Init();

	LOG_DEBUG("Renderer Initialized!");
}

void rujin::Renderer::Render() const
{
	const auto& color = GetBackgroundColor();
	SDL_SetRenderDrawColor(m_Renderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(m_Renderer);

	auto& sceneManager = SceneManager::GetInstance();

	sceneManager.Render();

	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplSDL2_NewFrame(m_Window);
	ImGui::NewFrame();

	sceneManager.OnGui(m_Window);

	ImGui::Render();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

	SDL_RenderPresent(m_Renderer);
}

void rujin::Renderer::Destroy()
{
	if (SDL_GL_GetCurrentContext()) // if we have a context, it is initialized
	{
		ImGui_ImplOpenGL2_Shutdown();
		ImGui_ImplSDL2_Shutdown();
		ImGui::DestroyContext();
	}

	if (m_Renderer != nullptr)
	{
		SDL_DestroyRenderer(m_Renderer);
		m_Renderer = nullptr;
	}
}

void rujin::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	SDL_QueryTexture(texture.GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

void rujin::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y, const float width, const float height) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	dst.w = static_cast<int>(width);
	dst.h = static_cast<int>(height);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}


SDL_Renderer* rujin::Renderer::GetSDLRenderer() const { return m_Renderer; }
const SDL_Color& rujin::Renderer::GetBackgroundColor() const { return m_ClearColor; }
void rujin::Renderer::SetBackgroundColor(const SDL_Color& color) { m_ClearColor = color; }
