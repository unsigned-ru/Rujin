#include "RujinPCH.h"
#include "GameObjectFactory.h"
#include "RectTransformComponent.h"

std::shared_ptr<rujin::GameObject> rujin::GameObjectFactory::CreateEmpty(const std::string& name)
{
	return std::shared_ptr<GameObject>(new GameObject(name));
}

std::shared_ptr<rujin::GameObject> rujin::GameObjectFactory::CreateDefault(const std::string& name)
{
	auto defaultGO{ std::shared_ptr<GameObject>(new GameObject(name))};
	defaultGO->AddComponent<TransformComponent>();

	return defaultGO;
}

std::shared_ptr<rujin::GameObject> rujin::GameObjectFactory::CreateCanvas(const std::string& name)
{
	auto canvas{ std::shared_ptr<GameObject>(new GameObject(name)) };
	canvas->AddComponent<RectTransformComponent>();

	return canvas;
}
