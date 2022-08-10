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
	, m_pCollisionQuadTree(std::make_unique<CollisionQuadTree>(collisionTreeBounds))
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
	for (const auto& pObject : m_GameObjects)
	{
		//apply construction-time transformations.
		pObject->GetTransform().UpdateSelfAndChildren();

		//start.
		pObject->Start();
	}
}

void Scene::LateStart()
{
	for (const auto& pObject : m_GameObjects)
	{
		pObject->LateStart();
	}
}

void Scene::Update()
{
	for (const auto& pObject : m_GameObjects)
	{
		pObject->Update();
	}
}

void rujin::Scene::FixedUpdate()
{
	for (const auto& pObject : m_GameObjects)
	{
		pObject->FixedUpdate();
	}

	m_pCollisionQuadTree->HandleCollision(m_pCollisionQuadTree.get());

}

void Scene::OnGui(SDL_Window* pWindow)
{
	for (const auto& pObject : m_GameObjects)
	{
		pObject->OnGui(pWindow);
	}
}

void Scene::Draw() const
{
	glPushMatrix();

	m_pActiveCamera->Project();

	for (const auto& pObject : m_GameObjects)
	{
		//update global transforms before drawing
		pObject->GetTransform().UpdateSelfAndChildren();

		pObject->Draw();
	}

	m_pCollisionQuadTree->DrawDebug();

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
	return m_pCollisionQuadTree.get();
}