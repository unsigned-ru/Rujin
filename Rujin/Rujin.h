#pragma once
struct SDL_Window;
namespace rujin
{
	class Rujin
	{
	public:
		void Initialize();
		void LoadGame() const;
		void Cleanup();
		void Run();
	private:
		static constexpr int s_MsPerFrame = 16; //16 for 60 fps, 33 for 30 
		static constexpr float s_FixedTimestamp = 0.02f;

		SDL_Window* m_Window{};

	};
}