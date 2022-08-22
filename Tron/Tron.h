#ifndef TRON_H
#define TRON_H

#include "GridGraph.h"
#include "IGame.h"

#pragma region Forward Declarations
namespace rujin
{
	class GameObject;
	class Scene;
	class ResourceService;
	class AudioService;
}

#pragma endregion


class Tron final : public IGame
{
public:
	enum class GameMode
	{
		SINGLE_PLAYER,
		CO_OP,
		PLAYER_VS_PLAYER
	};

	explicit Tron() = default;
	~Tron() override = default;

	Tron(const Tron&) = delete;
	Tron(Tron&&) noexcept = delete;
	Tron& operator=(const Tron&) = delete;
	Tron& operator=(Tron&&) noexcept = delete;

	void Configure(settings::InitParams& params) override;
	void Load() override;

	const glm::ivec2& GetGridDimensions() const;

	void StartGame(GameMode gamemode);
	void SwitchToNextLevel();
	void LoadHighScoreScene(uint32_t newScore);
	void LoadGameOverScene();


	GameMode GetGameMode() const;
private:
	void LoadMenuScreen();

	void LoadLevel1();
	void SpawnLevel1Enemies(Scene* pScene);

	void LoadLevel2();
	void SpawnLevel2Enemies(Scene* pScene);

	void LoadLevel3();
	void SpawnLevel3Enemies(Scene* pScene);


	std::vector<uint32_t> LoadHighScoresFromFile(const std::string& filename);

	void GenerateLevelGridFromTexture(Scene* pScene, const std::string& levelTexturePath, graph::GridGraph<graph::GridTerrainNode, graph::GraphConnection>* pGraph);
	void CreateLevelBoundsColliders(Scene* pScene);

	/*Level Grid*/
	const glm::vec2 m_GridStart{ 15, 16 };
	const glm::ivec2 m_GridDimensions{ 29, 26 };
	const float m_CellSize = 25.f;
	const float m_BoundsColliderSize{ 30.f };

	uint8_t m_CurrentLevel = 1;

	GameMode m_GameMode{};
};


#endif // Include Guard: TRON_H
