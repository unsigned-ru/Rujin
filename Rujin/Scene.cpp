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
	, m_pCollisionQuadTree(std::make_unique<CollisionQuadTree>(collisionTreeBounds))
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
	m_GameObjects.Iterate
	(
		[](const std::unique_ptr<GameObject>& pGameObject)
		{
			//apply construction-time transformations.
			pGameObject->GetTransform().UpdateSelfAndChildren();

			//start.
			pGameObject->Start();
		}
	);

	m_IsStarted = true;
}

void Scene::LateStart()
{
	m_GameObjects.Iterate
	(
		[](const std::unique_ptr<GameObject>& pGameObject)
		{
			pGameObject->LateStart();
		}
	);
}

void Scene::Update()
{
	m_ActiveGameObjects.Iterate
	(
		[](GameObject* pGameObject)
		{
			pGameObject->Update();
		}
	);
}

void Scene::FixedUpdate()
{
	m_ActiveGameObjects.Iterate
	(
		[](GameObject* pGameObject)
		{
			pGameObject->FixedUpdate();
		}
	);

	//ensure transforms are up to date for collision checks.
	m_ActiveGameObjects.Iterate
	(
		[](GameObject* pGameObject)
		{
			pGameObject->GetTransform().UpdateSelfAndChildren();
		}
	);

	m_pCollisionQuadTree->HandleCollision(m_pCollisionQuadTree.get());
}

void Scene::OnGui(SDL_Window* pWindow)
{
	m_ActiveGameObjects.Iterate
	(
		[pWindow](GameObject* pGameObject)
		{
			pGameObject->OnGui(pWindow);
		}
	);
}

void Scene::Draw() const
{
	glPushMatrix();

	m_pActiveCamera->Project();

	m_ActiveGameObjects.Iterate
	(
		[](const GameObject* pGameObject)
		{
			pGameObject->Draw();
		}
	);

	m_pCollisionQuadTree->DrawDebug();

	glPopMatrix();
}

void Scene::Destroy()
{
	delete m_pDefaultCamera;
}

void Scene::AddGameObject(GameObject* gameObject)
{
	gameObject->SetScene(this);

	if (m_IsStarted) //if we have already started, we are adding the GO at runtime... call start.
	{
		gameObject->GetTransform().UpdateSelfAndChildren();
		gameObject->Start();
	}

	m_GameObjects.GetVector().emplace_back(std::unique_ptr<GameObject>(gameObject));
	m_ActiveGameObjects.GetVector().emplace_back(gameObject);
}

void Scene::RemoveGameObject(GameObject* gameObject)
{
	if (gameObject->IsEnabled())
		m_ActiveGameObjects.Remove(gameObject);
	else
		m_InactiveGameObjects.Remove(gameObject);

	m_GameObjects.RemoveIf([gameObject](const std::unique_ptr<GameObject>& pObject) { return pObject.get() == gameObject; });
}

DeferredRemovalVector<GameObject*>& Scene::GetEnabledGameObjects()
{
	return m_ActiveGameObjects;
}

DeferredRemovalVector<GameObject*>& Scene::GetDisabledGameObjects()
{
	return m_InactiveGameObjects;
}


CollisionQuadTree* Scene::GetCollisionQuadTree() const
{
	return m_pCollisionQuadTree.get();
}

bool Scene::HasStarted() const
{
	return m_IsStarted;
}
