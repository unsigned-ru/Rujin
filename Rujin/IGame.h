#pragma once

namespace rujin
{
#pragma region Forward Declarations
	namespace settings { struct InitParams; };
#pragma endregion

	class IGame
	{
	public:
		explicit IGame() = default;
		virtual ~IGame() = default;

		IGame(const IGame&) = delete;
		IGame(IGame&&) noexcept = delete;
		IGame& operator=(IGame&&) noexcept = delete;
		IGame& operator=(const IGame&) = delete;

		//Pre-launch configuration.
		virtual void Configure(settings::InitParams& params) = 0;

		//Loading game.
		virtual void Load() = 0;
	};
}