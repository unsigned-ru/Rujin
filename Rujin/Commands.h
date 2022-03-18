#pragma once
#include "Components/Component.h"

namespace rujin
{
	class Component;
	class PeterPepperComponent;

	namespace command
	{
		class Base
		{
		public:
			explicit Base(Component* pComp) : m_pComponent(pComp) {}
			virtual ~Base() = default;
			virtual void Execute() = 0;

		protected:
			template<typename ComponentType>
			ComponentType* GetComponent() const { return static_cast<ComponentType*>(m_pComponent); }

		private:
			Component* m_pComponent;
		};

		class Die : public Base
		{
		public:
			explicit Die(PeterPepperComponent* pComp);
			void Execute() override;
		};

		class GainPoints : public Base
		{
		public:
			explicit GainPoints(PeterPepperComponent* pComp);
			void Execute() override;
		};
	}
}
