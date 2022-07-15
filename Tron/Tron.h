#pragma once
#include "IGame.h"

#pragma region Forward Declarations
namespace rujin
{
	class AudioService;
	class ResourceManager;

	namespace settings { struct InitializationParameters; }
}
#pragma endregion

class Tron final : public rujin::IGame
{
public:
	void Configure(rujin::settings::InitializationParameters& params) override;
	void Load() override;

private:
	void LoadTextures(rujin::ResourceManager& rm);
	void LoadFonts(rujin::ResourceManager& rm);
	void LoadAudio(rujin::AudioService& as);
};
