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

	m_GameObjects.SetElementAddedCallback
	(
		[this](const std::unique_ptr<GameObject>& pObj) 
		{
			//apply construction-time transformations.
			pObj->SetScene(this);
			pObj->GetTransform().UpdateSelfAndChildren();
			pObj->Start();
		}
	);

	m_GameObjects.SetElementRemovedCallback
	(
		[](const GameObject* pObj)
		{
			LOG_DEBUG_("Sucessfully Removed GO: {}", pObj->GetName());
		}
	);
}

Scene::~Scene()
{
	delete m_pDefaultCamera;
}

void Scene::Start()
{
	//starting of children is handled by process pending. (DeferredVector)
	m_IsStarted = true;
}

void Scene::LateStart()
{
	for (const std::unique_ptr<GameObject>& pGameObject : m_GameObjects.GetVector())
	{
		pGameObject->LateStart();
	}
}

void Scene::Update()
{
	for (GameObject* pGameObject : m_ActiveGameObjects.GetVector())
	{
		pGameObject->Update();
	}
}

void Scene::FixedUpdate()
{
	for (GameObject* pGameObject : m_ActiveGameObjects.GetVector())
	{
		pGameObject->FixedUpdate();
	}


	for (GameObject* pGameObject : m_ActiveGameObjects.GetVector())
	{
		//ensure global transforms are up-to-date before doing collision checks.
		pGameObject->GetTransform().UpdateSelfAndChildren();
	}

	m_pCollisionQuadTree->HandleCollision(m_pCollisionQuadTree.get());
}

void Scene::ProcessAdditionsAndRemovals()
{
	LOG_DEBUG("Processing removals and additions");
	m_InactiveGameObjects.ProcessRemovals();
	m_ActiveGameObjects.ProcessRemovals();
	m_GameObjects.ProcessRemovals();

	for (const std::unique_ptr<GameObject>& pObject : m_GameObjects.GetElementsToAdd())
	{
		pObject->ProcessAdditionsAndRemovals();
	}

	for (const std::unique_ptr<GameObject>& pObject : m_GameObjects.GetVector())
	{
		pObject->ProcessAdditionsAndRemovals();
	}

	m_InactiveGameObjects.ProcessAdditions();
	m_ActiveGameObjects.ProcessAdditions();
	m_GameObjects.ProcessAdditions();


}

void Scene::OnGui(SDL_Window* pWindow)
{
	for (GameObject* pGameObject : m_ActiveGameObjects.GetVector())
	{
		pGameObject->OnGui(pWindow);
	}
}

void Scene::Draw() const
{
	glPushMatrix();

	m_pActiveCamera->Project();

	for (const GameObject* pGameObject : m_ActiveGameObjects.GetVector())
	{
		pGameObject->Draw();
	}

	m_pCollisionQuadTree->DrawDebug();

	glPopMatrix();
}

void Scene::AddGameObject(GameObject* gameObject)
{
	m_GameObjects.PendPushBack(std::unique_ptr<GameObject>(gameObject));
	m_ActiveGameObjects.PendPushBack(gameObject);
}

void Scene::RemoveGameObject(GameObject* gameObject)
{
	if (gameObject->IsEnabled())
		m_ActiveGameObjects.PendRemove(gameObject);
	else
		m_InactiveGameObjects.PendRemove(gameObject);

	m_GameObjects.PendRemove(std::ranges::find_if(m_GameObjects.GetVector(), [gameObject](const std::unique_ptr<GameObject>& pObject) { return pObject.get() == gameObject; })->get());
}

DeferredVector<GameObject*, GameObject*>& Scene::GetEnabledGameObjects()
{
	return m_ActiveGameObjects;
}

DeferredVector<GameObject*, GameObject*>& Scene::GetDisabledGameObjects()
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
