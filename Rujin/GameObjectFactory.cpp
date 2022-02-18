#include "RujinPCH.h"
#include "GameObjectFactory.h"
#include "RectTransformComponent.h"

std::shared_ptr<rujin::GameObject> rujin::GameObjectFactory::CreateEmpty(const std::string& name)
{
	auto go = std::shared_ptr<GameObject>(new GameObject(name));
	go->AddComponent<TransformComponent>();
	return go;
}

std::shared_ptr<rujin::GameObject> rujin::GameObjectFactory::CreateCanvas(const std::string& name)
{
	auto go = std::shared_ptr<GameObject>(new GameObject(name));
	go->AddComponent<RectTransformComponent>();
	return go;
}
