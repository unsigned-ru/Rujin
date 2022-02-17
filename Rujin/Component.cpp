#include "RujinPCH.h"
#include "Component.h"
#include "GameObject.h"


rujin::Component::Component(const std::weak_ptr<GameObject> gameObject)
	: MonoBehaviour()
	, m_GameObject(gameObject)
{
}

void rujin::Component::Start()
{
}

void rujin::Component::Update()
{
}

void rujin::Component::FixedUpdate()
{
}

void rujin::Component::Render() const
{
}

void rujin::Component::Destroy()
{
}

std::weak_ptr<rujin::GameObject> rujin::Component::GetGameObject() const
{
	return m_GameObject;
}