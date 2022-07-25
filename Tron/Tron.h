#pragma once
#include "IGame.h"

#pragma region Forward Declarations
namespace rujin
{
	class AudioService;
	class ResourceManager;

	namespace settings { struct InitParams; }
}
#pragma endregion

class Tron final : public rujin::IGame
{
public:
	void Configure(rujin::settings::InitParams& params) override;
	void Load() override;

private:
	void LoadTextures(rujin::ResourceManager* rm);
	void LoadFonts(rujin::ResourceManager* rm);
	void LoadAudio(rujin::AudioService& as);

	void CreateTestObject();
};