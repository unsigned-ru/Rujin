#ifndef TRON_H
#define TRON_H

#include "IGame.h"

#pragma region Forward Declarations
namespace rujin
{
	class Scene;
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
	void GenerateLevelGridFromTexture(Scene* pScene, const std::string& levelTexturePath);
	void CreateLevelBoundsColliders(Scene* pScene);

	/*Level Grid*/
	const glm::vec2 m_GridStart{ 15, 16 };
	const float m_CellSize = 25.f;
	const glm::ivec2 m_GridDimensions{ 29, 26 };
	const float m_BoundsColliderSize{ 30.f };
};


#endif // Include Guard: TRON_H
