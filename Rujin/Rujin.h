#pragma once

struct SDL_Window;
namespace rujin
{
#pragma region Forward Declarations
	class IGame;
#

	namespace settings
	{
		enum class VSyncMode : int { ON = 1, OFF = 0, ADAPTIVE = -1 };

		struct InitializationParameters
		{
			std::string windowTitle = "Rujin - Game Engine";
			glm::ivec2 windowSize = { 630 , 480 };
			std::string resourcePath = "../Data/";
			VSyncMode vsyncMode = VSyncMode::ON;
			float fixedUpdateTimestep = 0.02f;
		};
	}

	class Rujin
	{
	public:
		explicit Rujin(IGame* pGame);

		void Initialize();
		void Cleanup();
		void Run();

		static float GetDeltaTime();

		static void SetVSync(settings::VSyncMode vsyncMode);

	private:
		void InitializeSDL();
		static void PrintSDLVersion();

		float m_FixedUpdateTimestep;

		static float s_DeltaTime;

		SDL_Window* m_Window{};
		glm::ivec2 m_WindowSize{};

		IGame* m_pGame = nullptr;

		settings::InitializationParameters m_InitParams{};
	};
}