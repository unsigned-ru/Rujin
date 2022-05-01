#pragma once
#include "Component.h"
#include "Subject.h"

namespace rujin
{
	class TextureRenderComponent;
	class TextComponent;

	namespace event
	{
		class IObserver;
	}

	class PeterPepperComponent final : public Component, public event::Subject
	{
	public:
		explicit PeterPepperComponent(const std::vector<event::IObserver*>& pObservers = {});
		~PeterPepperComponent() override = default;
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

