#include "RujinPCH.h"
#include "Renderer.h"

#include <algorithm>

#include "imgui.h"
#include "imgui_plot.h"
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

void rujin::Renderer::Init(SDL_Window * window)
{
	m_Window = window;
	m_Renderer = SDL_CreateRenderer(window, GetOpenGLDriverIndex(), SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (m_Renderer == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
	}

	//initialize ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplSDL2_InitForOpenGL(window, SDL_GL_GetCurrentContext());
	ImGui_ImplOpenGL2_Init();
}

void rujin::Renderer::Render()
{
	const auto& color = GetBackgroundColor();
	SDL_SetRenderDrawColor(m_Renderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(m_Renderer);

	SceneManager::GetInstance().Render();

	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplSDL2_NewFrame(m_Window);
	ImGui::NewFrame();
	ImGuiEx1();
	ImGuiEx2();
	ImGui::Render();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
	
	SDL_RenderPresent(m_Renderer);
}

void rujin::Renderer::Destroy()
{
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

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

void rujin::Renderer::ImGuiEx1()
{
	ImGui::Begin("Excercise 1", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	{
		ImGui::InputInt("# samples", &m_IntSamples);

		if (m_IntGenStatus != DataGenStatus::InProgress)
		{
			if (ImGui::Button("Trash the cache"))
			{
				m_IntGenStatus = DataGenStatus::InProgress;

				//clear old data
				m_IntDataX.clear();
				m_IntDataY.clear();

				int samples = m_IntSamples;

				//run thread to calculate
				std::thread([this, samples]()
					{
						const int bufferSize = static_cast<int>(powf(2, 26));
						auto* pBuffer = new int[bufferSize];

						for (int step = 1; step <= pow(2, samples); step *= 2)
						{
							const auto start = std::chrono::high_resolution_clock::now();

							for (int i = 0; i < bufferSize; i += step)
								pBuffer[i] *= 2;

							const auto end = std::chrono::high_resolution_clock::now();
							const auto total = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
							m_IntDataX.push_back(static_cast<float>(step));
							m_IntDataY.push_back(total / 100.f);
						}

						delete[] pBuffer;

						m_IntGenStatus = DataGenStatus::Finished;
					}).detach();
			}
		}
		else
		{
			ImGui::Text("Wait for it...");
		}

		if (m_IntGenStatus == DataGenStatus::Finished)
		{
			//plot data
			ImGui::PlotConfig conf;
			conf.values.xs = m_IntDataX.data(); // this line is optional
			conf.values.ys = m_IntDataY.data();
			conf.values.count = static_cast<int>(m_IntDataY.size());
			conf.scale.min = 0;
			conf.scale.max = std::ranges::max(m_IntDataY);
			conf.tooltip.show = true;
			conf.tooltip.format = "x=%.2f, y=%.2f";
			conf.grid_x.show = false;
			conf.grid_y.show = true;
			conf.frame_size = ImVec2(300, 200);
			conf.line_thickness = 2.f;

			ImGui::Plot("plot", conf);
		}
	}
	ImGui::End();
}

void rujin::Renderer::ImGuiEx2()
{
	ImGui::Begin("Excercise 2", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	{
		ImGui::InputInt("# samples", &m_GoSamples);

		if (m_GoGenStatus != DataGenStatus::InProgress)
		{
			if (ImGui::Button("Trash the cache with GameObject3D"))
			{
				m_GoGenStatus = DataGenStatus::InProgress;

				//clear old data
				m_GoDataX.clear();
				m_GoDataY.clear();

				int samples = m_GoSamples;

				//run thread to calculate
				std::thread([this, samples]()
					{
						const int bufferSize = static_cast<int>(powf(2, 26));
						auto* pBuffer = new GameObject3D[bufferSize];

						for (int step = 1; step <= pow(2, samples); step *= 2)
						{
							const auto start = std::chrono::high_resolution_clock::now();

							for (int i = 0; i < bufferSize; i += step)
								pBuffer[i].ID *= 2;

							const auto end = std::chrono::high_resolution_clock::now();
							const auto total = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
							m_GoDataX.push_back(static_cast<float>(step));
							m_GoDataY.push_back(total / 100.f);
						}

						delete[] pBuffer;

						m_GoGenStatus = DataGenStatus::Finished;
					}).detach();
			}
		}
		else
		{
			ImGui::Text("Wait for it...");
		}

		if (m_GoGenStatus == DataGenStatus::Finished)
		{
			//plot data
			ImGui::PlotConfig conf;
			conf.values.xs = m_GoDataX.data(); // this line is optional
			conf.values.ys = m_GoDataY.data();
			conf.values.count = static_cast<int>(m_GoDataY.size());
			conf.scale.min = 0;
			conf.scale.max = std::ranges::max(m_GoDataY);
			conf.tooltip.show = true;
			conf.tooltip.format = "x=%.2f, y=%.2f";
			conf.grid_x.show = false;
			conf.grid_y.show = true;
			conf.frame_size = ImVec2(300, 200);
			conf.line_thickness = 2.f;

			ImGui::Plot("plot", conf);
		}
	}
	{
		ImGui::InputInt("#samples", &m_GoAltSamples);

		if (m_GoAltGenStatus != DataGenStatus::InProgress)
		{
			if (ImGui::Button("Trash the cache with GameObject3DAlt"))
			{
				m_GoAltGenStatus = DataGenStatus::InProgress;

				//clear old data
				m_GoAltDataX.clear();
				m_GoAltDataY.clear();

				int samples = m_GoAltSamples;

				//run thread to calculate
				std::thread([this, samples]()
					{
						const int bufferSize = static_cast<int>(powf(2, 26));
						auto* pBuffer = new GameObject3DAlt[bufferSize];

						for (int step = 1; step <= pow(2, samples); step *= 2)
						{
							const auto start = std::chrono::high_resolution_clock::now();

							for (int i = 0; i < bufferSize; i += step)
								pBuffer[i].ID *= 2;

							const auto end = std::chrono::high_resolution_clock::now();
							const auto total = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
							m_GoAltDataX.push_back(static_cast<float>(step));
							m_GoAltDataY.push_back(total / 100.f);
						}

						delete[] pBuffer;

						m_GoAltGenStatus = DataGenStatus::Finished;
					}).detach();
			}
		}
		else
		{
			ImGui::Text("Wait for it...");
		}

		if (m_GoAltGenStatus == DataGenStatus::Finished)
		{
			//plot data
			ImGui::PlotConfig conf;
			conf.values.xs = m_GoAltDataX.data(); // this line is optional
			conf.values.ys = m_GoAltDataY.data();
			conf.values.count = static_cast<int>(m_GoAltDataY.size());
			conf.scale.min = 0;
			conf.scale.max = std::ranges::max(m_GoAltDataY);
			conf.tooltip.show = true;
			conf.tooltip.format = "x=%.2f, y=%.2f";
			conf.grid_x.show = false;
			conf.grid_y.show = true;
			conf.frame_size = ImVec2(300, 200);
			conf.line_thickness = 2.f;

			ImGui::Plot("plot", conf);
		}
	}

	ImGui::End();
}

SDL_Renderer* rujin::Renderer::GetSDLRenderer() const { return m_Renderer; }
const SDL_Color& rujin::Renderer::GetBackgroundColor() const { return m_ClearColor; }
void rujin::Renderer::SetBackgroundColor(const SDL_Color& color) { m_ClearColor = color; }
