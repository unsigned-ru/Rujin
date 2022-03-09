#include "RujinPCH.h"
#include "Scene.h"

using namespace rujin;

unsigned int Scene::m_IdCounter = 0;

Scene::Scene(const std::string& name)
	: IGameLoopObject()
	, m_Name(name)
	, m_GameObjects()
	, m_ActiveGameObjects()
	, m_InactiveGameObjects()
{}


void Scene::Start()
{
	for (const auto& object : m_GameObjects)
	{
		object->Start();
	}
}

void Scene::Update()
{
	for (const auto& object : m_GameObjects)
	{
		object->Update();
	}
}

void rujin::Scene::FixedUpdate()
{
	for (const auto& object : m_GameObjects)
	{
		object->FixedUpdate();
	}
}

void Scene::OnGui(SDL_Window* pWindow)
{
	for (const auto& object : m_GameObjects)
	{
		object->OnGui(pWindow);
	}
}

void Scene::Render() const
{
	for (const auto& object : m_GameObjects)
	{
		object->Render();
	}
}

void Scene::Destroy()
{
}

void Scene::AddGameObject(GameObject* gameObject)
{
	m_GameObjects.push_back(std::unique_ptr<GameObject>(gameObject));
}

