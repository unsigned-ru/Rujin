#include "RujinPCH.h"
#include "PeterPepperComponent.h"
#include "GameEvents.h"

using namespace rujin;

PeterPepperComponent::PeterPepperComponent(const std::vector<event::IObserver*>& pObservers) : Subject(pObservers) {}

void PeterPepperComponent::Start()
{
	Component::Start();
}

void PeterPepperComponent::Update()
{
	Component::Update();
}

void PeterPepperComponent::Die()
{
	if (m_Lives <= 0) //can't murder what's already dead 8)
		return;

	//remove a life
	--m_Lives;

	//notify about the loss of life :'(
	const event::PlayerDiedData data{GameObject(), m_Lives};
	Notify(event::Identifier::PlayerDied, &data);
}

void PeterPepperComponent::AddPoints(const uint32_t points)
{
	m_Points += points;

	const event::PlayerGainedPointsData data {GameObject(), m_Points};
	Notify(event::Identifier::PlayerGainedPoints, &data);
}

uint16_t PeterPepperComponent::GetLives() const
{
	return m_Lives;
}

uint32_t PeterPepperComponent::GetPoints() const
{
	return m_Points;
}


