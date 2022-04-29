#include "RujinPCH.h"
#include "SceneManager.h"
#include "Scene.h"

rujin::SceneManager::SceneManager() = default;
rujin::SceneManager::~SceneManager() = default;

void rujin::SceneManager::Start()
{
	for (const auto& scene : m_Scenes)
	{
		scene->Start();
	}
}

void rujin::SceneManager::LateStart()
{
	for (const auto& scene : m_Scenes)
	{
		scene->LateStart();
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

void rujin::SceneManager::OnGui(SDL_Window* pWindow)
{
	for (const auto& scene : m_Scenes)
	{
		scene->OnGui(pWindow);
	}
}


void rujin::SceneManager::Render() const
{
	for (const auto& scene : m_Scenes)
	{
		scene->Render();
	}
}

rujin::Scene* rujin::SceneManager::CreateScene(const std::string& name)
{
	auto* pScene = new rujin::Scene(name);
	m_Scenes.push_back(std::unique_ptr<rujin::Scene>(pScene));
	return pScene;
}

rujin::Scene* rujin::SceneManager::GetScene(const std::string& name) const
{
	const auto it = std::ranges::find_if
	(
		m_Scenes,
		[&name](const std::unique_ptr<Scene>& scene)
		{
			return scene->m_Name == name;
		}
	);

	if (it == m_Scenes.end())
		return nullptr;

	return it->get();
}

rujin::Scene* rujin::SceneManager::GetScene(const size_t idx) const
{
	//TODO: could possibly optimize
	try
	{
		return m_Scenes.at(idx).get();
	}
	catch (std::out_of_range& )
	{
		return nullptr;
	}
}