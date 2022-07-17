#pragma once

namespace rujin
{
	namespace settings { struct InitializationParameters; }

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
		virtual void Configure(settings::InitializationParameters& params) = 0;

		//Loading game.
		virtual void Load() = 0;
	};
}