#include "RujinPCH.h"
#include "EnemyComponent.h"

#include "PeterPepperComponent.h"
#include "GameEvents.h"

using namespace rujin;
//using namespace event;

EnemyComponent::EnemyComponent(const std::vector<event::IObserver*>& pObservers) : Subject(pObservers) {}

void EnemyComponent::Start()
{
	Component::Start();
}

void EnemyComponent::Update()
{
	Component::Update();
}

void EnemyComponent::Die(PeterPepperComponent* const pKilledBy)
{
	pKilledBy->AddPoints(100);
}
