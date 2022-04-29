#pragma once
struct SDL_Window;
namespace rujin
{
	class Rujin
	{
	public:
		/**
		 * \return returns false if initialization resulted in requested shutdown.
		 */
		void Initialize();
		void LoadGame() const;
		void Cleanup();
		void Run();

		static float GetDeltaTime();
	private:
		void InitializeSDL();
		void InitializeSteamworks();

		static constexpr int s_MsPerFrame = 16; //16 for 60 fps, 33 for 30 
		static constexpr float s_FixedTimestamp = 0.02f;
		static float s_DeltaTime;

		SDL_Window* m_Window{};
		glm::ivec2 m_WindowSize{};

	};
}