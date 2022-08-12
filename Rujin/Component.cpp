#include "RujinPCH.h"
#include "Component.h"

#include "GameObject.h"

using namespace rujin;

GameObject* Component::GameObject() const
{
	return m_pGameObject;
}

void Component::OnEnable()
{
	StatefullObject::OnEnable();

	rujin::GameObject* pGameObject = GameObject();

	auto goEnabledComponents = pGameObject->GetEnabledComponents().GetVector();
	if (std::find(goEnabledComponents.begin(), goEnabledComponents.end(), this) == goEnabledComponents.end())
	{
		goEnabledComponents.emplace_back(this);
		pGameObject->GetDisabledComponents().Remove(this);
	}
}

void Component::OnDisable()
{
	StatefullObject::OnDisable();

}
