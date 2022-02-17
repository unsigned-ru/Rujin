#include "RujinPCH.h"
#include "Scene.h"

using namespace rujin;

unsigned int Scene::m_IdCounter = 0;

Scene::Scene(const std::string& name)
	: MonoBehaviour()
	, m_Name(name)
	, m_GameObjects()
	, m_ActiveGameObjects()
	, m_InactiveGameObjects()
{}

Scene::~Scene() = default;


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

std::weak_ptr<GameObject> Scene::AddGameObject(const std::shared_ptr<GameObject> gameObject)
{
	m_GameObjects.push_back(gameObject);
	return gameObject;
}

