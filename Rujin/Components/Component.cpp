#include "RujinPCH.h"
#include "Components/Component.h"

using namespace rujin;

Component::Component()
{
}

GameObject* Component::GameObject() const
{
	return m_pGameObject;
}
