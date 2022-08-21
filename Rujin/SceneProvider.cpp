#include "RujinPCH.h"
#include "SceneProvider.h"

#include "Rujin.h"
#include "Scene.h"

rujin::SceneProvider::SceneProvider() = default;
rujin::SceneProvider::~SceneProvider() = default;

void rujin::SceneProvider::Start()
{
	//start all scenes
	for (const auto& scene : m_Scenes)
	{
		scene->Start();
	}
}

void rujin::SceneProvider::LateStart()
{
	//late start all scenes
	for (const auto& scene : m_Scenes)
	{
		scene->LateStart();
	}
}

void rujin::SceneProvider::HandleSceneSwitch()
{
	if (m_pActiveSceneToSet != nullptr)
	{
  		m_pActiveScene = m_pActiveSceneToSet;
		m_pActiveSceneToSet = nullptr;
	}
}

void rujin::SceneProvider::Update()
{
	m_pActiveScene->Update();
}

void rujin::SceneProvider::FixedUpdate()
{
	m_pActiveScene->FixedUpdate();
}

void rujin::SceneProvider::ProcessAdditionsAndRemovalsForAllScenes()
{
	//late start all scenes
	for (const auto& scene : m_Scenes)
	{
		scene->ProcessAdditionsAndRemovals();
	}
}

void rujin::SceneProvider::ProcessAdditionsAndRemovals()
{
	m_pActiveScene->ProcessAdditionsAndRemovals();
}

void rujin::SceneProvider::OnGui(SDL_Window* pWindow)
{
	m_pActiveScene->OnGui(pWindow);
}


void rujin::SceneProvider::Draw() const
{
	m_pActiveScene->Draw();
}

rujin::Scene* rujin::SceneProvider::CreateScene(const std::string& name, const Rectf& collisionTreeBounds, Camera* pCamera)
{
	auto* pScene = new Scene(name, collisionTreeBounds, pCamera);
	m_Scenes.push_back(std::unique_ptr<rujin::Scene>(pScene));

	if (m_Scenes.size() == 1)
		SetActiveScene(pScene);

	return pScene;
}

rujin::Scene* rujin::SceneProvider::GetScene(const std::string& name) const
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

void rujin::SceneProvider::SetActiveScene(Scene* pScene)
{
	m_pActiveSceneToSet = pScene;
}

void rujin::SceneProvider::SetActiveScene(const std::string& name)
{
	SetActiveScene(GetScene(name));
}

rujin::Scene* rujin::SceneProvider::GetActiveScene() const
{
	return m_pActiveScene;
}
