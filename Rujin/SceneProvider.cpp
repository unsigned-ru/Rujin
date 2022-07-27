#include "RujinPCH.h"
#include "SceneProvider.h"

#include "Camera.h"
#include "Rujin.h"
#include "Scene.h"

rujin::SceneProvider::SceneProvider() = default;
rujin::SceneProvider::~SceneProvider() = default;

void rujin::SceneProvider::Start()
{
	for (const auto& scene : m_Scenes)
	{
		scene->Start();
	}
}

void rujin::SceneProvider::LateStart()
{
	for (const auto& scene : m_Scenes)
	{
		scene->LateStart();
	}
}

void rujin::SceneProvider::Update()
{
	for (const auto& scene : m_Scenes)
	{
		scene->Update();
	}
}

void rujin::SceneProvider::FixedUpdate()
{
	for (const auto& scene : m_Scenes)
	{
		scene->FixedUpdate();
	}
}

void rujin::SceneProvider::OnGui(SDL_Window* pWindow)
{
	for (const auto& scene : m_Scenes)
	{
		scene->OnGui(pWindow);
	}
}


void rujin::SceneProvider::Draw() const
{
	for (const auto& scene : m_Scenes)
	{
		scene->Draw();
	}
}

rujin::Scene* rujin::SceneProvider::CreateScene(const std::string& name, Camera* pCamera)
{
	auto* pScene = new Scene(name, pCamera);
	m_Scenes.push_back(std::unique_ptr<rujin::Scene>(pScene));
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

rujin::Scene* rujin::SceneProvider::GetScene(const size_t idx) const
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
