#include "RujinPCH.h"
#include "DearImGuiComponent.h"

#include <thread>

#include "imgui.h"
#include "imgui_plot.h"

#include "backends/imgui_impl_opengl2.h"
#include "backends/imgui_impl_sdl.h"

void rujin::DearImGuiComponent::Update()
{
	Component::Update();
}

void rujin::DearImGuiComponent::OnGui(SDL_Window* pWindow)
{
	Component::OnGui(pWindow);

	ImGuiEx1();
	ImGuiEx2();

}


void rujin::DearImGuiComponent::ImGuiEx1()
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

void rujin::DearImGuiComponent::ImGuiEx2()
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