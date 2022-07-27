#pragma once
#include "IGame.h"
#include "ResourceService.h"

#pragma region Forward Declarations
namespace rujin
{
	class ResourceService;
	class AudioService;
}

#pragma endregion


class Tron final : public rujin::IGame
{
public:
	void Configure(settings::InitParams& params) override;
	void Load() override;

private:
	void LoadTextures(ResourceService& rs);
	void LoadFonts(ResourceService& rs);
	void LoadAudio(AudioService& as);
};