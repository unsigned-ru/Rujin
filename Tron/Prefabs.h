#ifndef PREFABS_H
#define PREFABS_H
#include "TronPlayerComponent.h"

class SpawnLocationListComponent;

namespace rujin
{
	class GameObject;
}

namespace prefabs
{
	GameObject* CreatePlayerTank(const std::string& name, PlayerIndex playerIdx);

	GameObject* CreateEnemyTank();

	GameObject* CreateEnemyRecognizer();

	GameObject* CreatePlayerHUD(const std::string& name, TronPlayerComponent* pPlayer);

	GameObject* CreateRandomTeleporter(const std::string& name);
}

#endif // Include Guard: PREFABS_H
