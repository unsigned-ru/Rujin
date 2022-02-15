#include "RujinPCH.h"
#include "SceneManager.h"
#include "Scene.h"

void rujin::SceneManager::Start()
{
	for (const auto& scene : m_Scenes)
	{
		scene->Start();
	}
}

void rujin::SceneManager::Update()
{
	for (const auto& scene : m_Scenes)
	{
		scene->Update();
	}
}

void rujin::SceneManager::FixedUpdate()
{
	for (const auto& scene : m_Scenes)
	{
		scene->FixedUpdate();
	}
}

void rujin::SceneManager::Render() const
{
	for (const auto& scene : m_Scenes)
	{
		scene->Render();
	}
}

std::weak_ptr<rujin::Scene> rujin::SceneManager::CreateScene(const std::string& name)
{
	const auto& scene = std::shared_ptr<Scene>(new Scene(name));
	m_Scenes.push_back(scene);
	return std::weak_ptr(scene);
}

std::weak_ptr<rujin::Scene> rujin::SceneManager::GetScene(const std::string& name) const
{
	const auto it = std::ranges::find_if
	(
		m_Scenes,
		[&name](const std::shared_ptr<Scene> scene)
		{
			return scene->m_Name == name;
		}
	);

	if (it == m_Scenes.end())
		return std::weak_ptr<rujin::Scene>();

	return std::weak_ptr(*it);
}

std::weak_ptr<rujin::Scene> rujin::SceneManager::GetScene(const size_t idx) const
{
	//TODO: not sure if this is the best way to do it
	try
	{
		return m_Scenes.at(idx);
	}
	catch (std::out_of_range& )
	{
		return std::weak_ptr<rujin::Scene>();
	}
	
}
