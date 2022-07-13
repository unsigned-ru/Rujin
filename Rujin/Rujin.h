#pragma once

class IGame;
struct SDL_Window;
namespace rujin
{
	class Rujin
	{
		struct InitializationParameters
		{
			std::string resourcePath{"../Data/"};
		};

	public:
		void Initialize();
		void Cleanup();
		void Run(IGame* pGame, const InitializationParameters& engineParams = {});

		static float GetDeltaTime();
	private:
		void InitializeSDL();
		static void PrintSDLVersion();


		static constexpr int s_MsPerFrame = 16; //16 for 60 fps, 33 for 30 
		static constexpr float s_FixedTimestamp = 0.02f;
		static float s_DeltaTime;

		SDL_Window* m_Window{};
		glm::ivec2 m_WindowSize{};

	};
}