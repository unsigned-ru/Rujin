#pragma once
#include "Component.h"
#include "Subject.h"

namespace rujin
{
	class PeterPepperComponent;

	class EnemyComponent final : public Component, public event::Subject
	{
	public:
		explicit EnemyComponent(const std::vector<event::IObserver*>& pObservers = std::vector<event::IObserver*>());

		void Start() override;
		void Update() override;

		void Die(PeterPepperComponent* const pKilledBy);

	private:
	};
}

