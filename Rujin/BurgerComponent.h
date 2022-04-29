#pragma once
#include "Component.h"

#include "Subject.h"

namespace rujin
{
	class PeterPepperComponent;

	class BurgerComponent final : public Component, public event::Subject
	{
	public:
		explicit BurgerComponent(const std::vector<event::IObserver*>& pObservers = std::vector<event::IObserver*>());

		void Start() override;
		void Update() override;

		void Drop(PeterPepperComponent* const pDroppedBy);

	private:
	};
}

