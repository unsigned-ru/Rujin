#include "RujinPCH.h"
#include "Scene.h"

#include "Camera.h"
#include "CollisionQuadTree.h"
#include "Rujin.h"

using namespace rujin;

unsigned int Scene::m_IdCounter = 0;

Scene::Scene(const std::string& name, const Rectf& collisionTreeBounds, Camera* pCamera)
	: IGameLoopObject()
	, m_Name(name)
	, m_GameObjects()
	, m_CollisionQuadTree(std::make_unique<CollisionQuadTree>(collisionTreeBounds))
	, m_ActiveGameObjects()
	, m_InactiveGameObjects()
{
	if (!pCamera)
	{
		m_pDefaultCamera = new Camera(Rujin::Get()->GetWindowContext().windowSize);
		m_pActiveCamera = m_pDefaultCamera;
	}
	else
		m_pActiveCamera = pCamera;
}

Scene::~Scene()
{
	Destroy();
}

void Scene::Start()
{
	for (const auto& object : m_GameObjects)
	{
		object->Start();
	}
}

void Scene::LateStart()
{
	for (const auto& object : m_GameObjects)
	{
		object->LateStart();
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

void Scene::Draw() const
{
	glPushMatrix();

	m_pActiveCamera->Project();

	for (const auto& object : m_GameObjects)
	{
		object->Draw();
	}

	m_CollisionQuadTree->DrawDebug();

	glPopMatrix();
}

void Scene::Destroy()
{
	delete m_pDefaultCamera;
}

void Scene::AddGameObject(std::unique_ptr<GameObject>& gameObject)
{
	gameObject->SetScene(this);
	m_GameObjects.push_back(std::move(gameObject));
}

CollisionQuadTree* Scene::GetCollisionQuadTree() const
{
	return m_CollisionQuadTree.get();
}