#ifndef RUJIN_H
#define RUJIN_H
#include "Singleton.h"
#include "Structs.h"

#include <random>

struct SDL_Window;
namespace rujin
{
#pragma region Forward Declarations
	class IGame;
#pragma endregion

	namespace settings
	{
		struct InitParams
		{
			std::string windowTitle = "Rujin - Game Engine";
			glm::ivec2 windowSize = { 630 , 480 };
			std::string resourcePath = "../Data/";
			VSyncMode vsyncMode = VSyncMode::ON;
			float fixedUpdateTimestep = 0.015f;
			glm::vec4 glClearColor{ 0.f, 0.f, 0.f, 1.f };
		};
	}

	class Rujin final : public Singleton<Rujin>
	{
	public:
		Rujin(const Rujin&) = delete;
		Rujin(Rujin&&) noexcept = delete;
		Rujin& operator=(const Rujin&) = delete;
		Rujin& operator=(Rujin&&) noexcept = delete;

		void Run();

		const WindowContext& GetWindowContext() const { return m_WindowContext;  }
		WindowContext& GetWindowContext() { return m_WindowContext;  }
		const settings::InitParams& GetInitParams() const { return m_InitParams; }
		float GetDeltaTime() const { return m_DeltaTime; }
		float GetFixedUpdateDeltaTime() const { return m_FixedUpdateTimestep; }
		float GetTotalTime() const { return m_TotalTime; }


		std::default_random_engine& GetRandomEngine();

		void SetVSync(VSyncMode vsyncMode);

		template<typename Game>
		Game* GetGame() const requires std::is_base_of_v<IGame, Game>
		{
			return dynamic_cast<Game*>(m_pGame);
		}

	private:
		friend class Singleton<Rujin>;
		explicit Rujin(IGame* pGame);
		~Rujin() override;
		void Initialize() override;

		WindowContext m_WindowContext{};
		settings::InitParams m_InitParams{};

		void InitializeSDL();
		void PrintSDLVersion();

		float m_FixedUpdateTimestep;
		float m_DeltaTime{};
		float m_TotalTime{};

		std::default_random_engine m_RandomEngine{};

		IGame* m_pGame = nullptr;
	};
}

#endif