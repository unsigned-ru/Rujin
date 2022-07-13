#pragma once

class IGame
{
public:
	explicit IGame() = default;
	virtual ~IGame() = default;
	IGame(const IGame&) = delete;
	IGame(IGame&&) noexcept = delete;
	IGame& operator=(IGame&&) noexcept = delete;
	IGame& operator=(const IGame&) = delete;

	virtual void Load() = 0;
};
