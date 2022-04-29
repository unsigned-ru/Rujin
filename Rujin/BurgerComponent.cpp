#include "RujinPCH.h"
#include "BurgerComponent.h"

#include "PeterPepperComponent.h"
#include "GameEvents.h"

using namespace rujin;

BurgerComponent::BurgerComponent(const std::vector<event::IObserver*>& pObservers) : Subject(pObservers) {}

void BurgerComponent::Start()
{
	Component::Start();
}

void BurgerComponent::Update()
{
	Component::Update();
}

void BurgerComponent::Drop(PeterPepperComponent* const pDroppedBy)
{
	pDroppedBy->AddPoints(50);
}
