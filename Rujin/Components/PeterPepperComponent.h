#pragma once
#include "Components/Component.h"
#include "Events/ObserverPattern/Interface/ISubject.h"

namespace rujin
{
	class TextureRenderComponent;
	class TextComponent;

	class PeterPepperComponent final : public Component, public event::ISubject
	{
	public:
		explicit PeterPepperComponent() = default;

		void Start() override;
		void Update() override;

		void Die();
		void AddPoints(const uint32_t points);

		uint16_t GetLives() const;
		uint32_t GetPoints() const;

	private:
		uint16_t m_Lives{ 3 };
		uint32_t m_Points{ 0 };
	};
}

