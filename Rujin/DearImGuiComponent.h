#pragma once
#include "Component.h"

namespace rujin
{
	class DearImGuiComponent : public Component
	{
	public:
		void Update() override;
		void OnGui(SDL_Window* pWindow) override;

	private:

		//test data structures
		enum class DataGenStatus
		{
			Uninitialized,
			InProgress,
			Finished
		};
		struct Transform
		{
			float matrix[16] = {
				1,0,0,0,
				0,1,0,0,
				0,0,1,0,
				0,0,0,1 };
		};
		class GameObject3D
		{
		public:
			Transform transform;
			int ID;
		};
		class GameObject3DAlt
		{
		public:
			Transform* transform;
			int ID;
		};

		//tests
		void ImGuiEx1();
		void ImGuiEx2();

		int m_IntSamples{ 10 };
		DataGenStatus m_IntGenStatus{ DataGenStatus::Uninitialized };
		std::vector<float> m_IntDataX{};
		std::vector<float> m_IntDataY{};

		int m_GoSamples{ 10 };
		DataGenStatus m_GoGenStatus{ DataGenStatus::Uninitialized };
		std::vector<float> m_GoDataX{};
		std::vector<float> m_GoDataY{};

		int m_GoAltSamples{ 10 };
		DataGenStatus m_GoAltGenStatus{ DataGenStatus::Uninitialized };
		std::vector<float> m_GoAltDataX{};
		std::vector<float> m_GoAltDataY{};
	};
}

