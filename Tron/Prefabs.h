#ifndef PREFABS_H
#define PREFABS_H
#include "TronPlayerComponent.h"

namespace rujin
{
	class GameObject;
}

namespace prefabs
{
	GameObject* CreatePlayerTank(const std::string& name, PlayerIndex playerIdx);

	GameObject* CreateEnemyTank();

	GameObject* CreateEnemyRecognizer();
}

#endif // Include Guard: PREFABS_H
